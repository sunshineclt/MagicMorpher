//
//  main.cpp
//  MagicMorpher
//
//  Created by 陈乐天 on 2017/1/16.
//  Copyright © 2017年 陈乐天. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

#include "FaceLandmarkGetter.hpp"
#include "Trianglationer.hpp"

using namespace std;
using namespace cv;

// apply affine transform calculated using srcTri and dstTri to src
void applyAffineTransform(Mat & warpImage, Mat & src, std::vector<Point2f> & srcTri, std::vector<Point2f> & dstTri) {
    // given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform(srcTri, dstTri);
    // apply the Affine Transform just found to the src image
    warpAffine(src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}

// warps and alpha blends triangular regions from img1 and img2 to img
void morphTriangle(Mat & img1, Mat & img2, Mat & img, std::vector<Point2f> & t1, std::vector<Point2f> & t2, std::vector<Point2f> & t, double alpha) {
    // find bounding rectangle for each triangle
    Rect r = boundingRect(t);
    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);
    
    // offset points by left top corner of the respective rectangles
    std::vector<Point2f> t1Rect, t2Rect, tRect;
    std::vector<Point> tRectInt;
    for (int i = 0; i < 3; ++i) {
        tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
        tRectInt.push_back( Point(t[i].x - r.x, t[i].y - r.y) ); // for fillConvexPoly
        t1Rect.push_back( Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back( Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
    }
    
    // get mask by filling triangle
    Mat mask = Mat::zeros(r.height, r.width, CV_32FC3);
    fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);
    
    // apply warpImage to small rectangular patches
    Mat img1Rect, img2Rect;
    img1(r1).copyTo(img1Rect);
    img2(r2).copyTo(img2Rect);
    
    Mat warpImage1 = Mat::zeros(r.height, r.width, img1Rect.type());
    Mat warpImage2 = Mat::zeros(r.height, r.width, img2Rect.type());
    
    applyAffineTransform(warpImage1, img1Rect, t1Rect, tRect);
    applyAffineTransform(warpImage2, img2Rect, t2Rect, tRect);
    
    // alpha blend rectangular patches
    Mat imgRect = (1.0 - alpha) * warpImage1 + alpha * warpImage2;
    
    // copy triangular region of the rectangular patch to the output image
    multiply(imgRect,mask, imgRect);
    multiply(img(r), Scalar(1.0,1.0,1.0) - mask, img(r));
    img(r) = img(r) + imgRect;
}

int main(int argc, char** argv) {
    
    if (argc == 1) {
        cout << "Call this program like this:" << endl;
        cout << "./MagicMorpher /path/shape_predictor_68_face_landmarks.dat /path/1.jpg /path/2.jpg /path/target.jpg 0.5 1 0" << endl;
        cout << "The last three parameter means: " << endl;
        cout << "1. Alpha. The bigger alpha, the morphed picture more like pic1. " << endl;
        cout << "2. Whether addBoundary(0-no, 1-yes). If not, then the morphed pic will only have face but not the surroundings like hair. " << endl;
        cout << "3. Whether demoMode(0-no, 1-yes). If yes, then it will demonstrate the landmarks found and trianglation result. " << endl;
        return 0;
    }
    
    // parameter decoding
    string shape_predictor_path(argv[1]);
    string pic1_path(argv[2]);
    string pic2_path(argv[3]);
    string target_path(argv[4]);
    double alpha = atof(argv[5]);
    bool addBoundary = false;
    if (argv[6][0] == '1') {
        addBoundary = true;
    }
    bool isDemoMode = false;
    if (argv[7][0] == '1') {
        isDemoMode = true;
    }
    bool isDebugMode = false;
    
    // read image and get prepared
    Mat img1 = imread(pic1_path);
    Mat img2 = imread(pic2_path);
    img1.convertTo(img1, CV_32F);
    img2.convertTo(img2, CV_32F);
    Mat imgMorph = Mat::zeros(img1.size(), CV_32FC3);
    Scalar delaunay_color(255,255,255), points_color(0, 0, 255);
    
    // detect face landmarks on pic1
    FaceLandmarkGetter landmarkGetter1(shape_predictor_path, pic1_path);
    std::vector<cv::Point2f> points1 = landmarkGetter1.getFaceLandMark(isDemoMode);
    Trianglationer trier1(img1, points1);
    if (addBoundary) {
        trier1.add_boundary();
    }
    if (isDebugMode) {
        trier1.display_points();
    }
    if (isDemoMode) {
        trier1.do_trianglation();
        trier1.show_trianglation(delaunay_color, points_color);
    }
    
    // detect face aldnmarks on pic2
    FaceLandmarkGetter landmarkGetter2(shape_predictor_path, pic2_path);
    std::vector<cv::Point2f> points2 = landmarkGetter2.getFaceLandMark(isDemoMode);
    Trianglationer trier2(img2, points2);
    if (addBoundary) {
        trier2.add_boundary();
    }
    if (isDebugMode) {
        trier2.display_points();
    }
    if (isDemoMode) {
        trier2.do_trianglation();
        trier2.show_trianglation(delaunay_color, points_color);
    }
    
    // determine the characteristic point of morphed picture
    std::vector<cv::Point2f> points;
    for (unsigned int i = 0; i < points1.size(); ++i) {
        float x = 0.5 * (points1[i].x + points2[i].x);
        float y = 0.5 * (points1[i].y + points2[i].y);
        points.push_back(Point2f(x, y));
    }
    
    // use the target picture's characteristic point to do trianglation(Delaunay Triangulation)
    Trianglationer trier(img1, points);
    if (addBoundary) {
        trier.add_boundary();
    }
    trier.do_trianglation();
    // get the trianglation result with the format (index[i][0], index[i][1] and index[i][2]) which form a triangle. Each one is a index of points.
    std::vector<std::vector<int> > index = trier.get_trianglation_index_result();
    
    // for every triangle, run triangle reconstruction using the input pic1 and pic2 to morph to target pic
    for (unsigned int i = 0; i < index.size(); ++i) {
        // t1 is triangle in pic1
        // t2 is triangle in pic2
        // t3 is triangle in target pic
        std::vector<Point2f> t1, t2, t;
        
        // get the index
        int x = index[i][0];
        int y = index[i][1];
        int z = index[i][2];
        
        // triangle's vertex's coordination for pic1
        t1.push_back(trier1.points[x]);
        t1.push_back(trier1.points[y]);
        t1.push_back(trier1.points[z]);
        
        // triangle's vertex's coordination for pic2
        t2.push_back(trier2.points[x]);
        t2.push_back(trier2.points[y]);
        t2.push_back(trier2.points[z]);
        
        // triangle's vertex's coordination for target pic
        t.push_back(trier1.points[x] * alpha + trier2.points[x] * (1 - alpha));
        t.push_back(trier1.points[y] * alpha + trier2.points[y] * (1 - alpha));
        t.push_back(trier1.points[z] * alpha + trier2.points[z] * (1 - alpha));
        
        morphTriangle(img1, img2, imgMorph, t1, t2, t, alpha);
    }
    
    imshow("Morphed Face", imgMorph / 255.0);
    cout << "focus on the image and press any key to continue..." << endl;
    waitKey(0);
    imwrite(target_path, imgMorph);
    return 0;
}
