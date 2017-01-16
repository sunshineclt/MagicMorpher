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

using namespace std;
using namespace cv;

// Draw a single point
static void draw_point( Mat& img, Point2f fp, Scalar color )
{
    circle( img, fp, 2, color, CV_FILLED, CV_AA, 0 );
}

// Draw delaunay triangles
static void draw_delaunay( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
    
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0,0, size.width, size.height);
    
    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        
        // Draw rectangles completely inside the image.
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
        }
    }
}

int main() {
    
    string shape_predictor_path = "/Users/cjjlt/programming/C++/MagicMorpher/shape_predictor_68_face_landmarks.dat";
    string pic1_path = "/Users/cjjlt/programming/C++/MagicMorpher/hillary_clinton.jpg";
    string pic2_path = "/Users/cjjlt/programming/C++/MagicMorpher/donald_trump.jpg";
    FaceLandmarkGetter landmarkGetter1(shape_predictor_path, pic1_path);
    std::vector<cv::Point2f> points1 = landmarkGetter1.getFaceLandMark();
    for (unsigned int i = 0; i < points1.size(); ++i) {
        cout << i << "th part: " << points1[i] << endl;
    }
    FaceLandmarkGetter landmarkGetter2(shape_predictor_path, pic2_path);
    std::vector<cv::Point2f> points2 = landmarkGetter2.getFaceLandMark();
    for (unsigned int i = 0; i < points2.size(); ++i) {
        cout << i << "th part: " << points2[i] << endl;
    }
    
    Scalar delaunay_color(255,255,255), points_color(0, 0, 255);
    
    Mat img1 = imread(pic1_path);
    Size size1 = img1.size();
    Rect rect1(0, 0, size1.width, size1.height);
    Subdiv2D subdiv1(rect1);
    for (std::vector<cv::Point2f>::iterator iterator = points1.begin(); iterator != points1.end(); ++iterator) {
        subdiv1.insert(*iterator);
    }
//    std::vector<Vec6f> triangleList1;
//    subdiv1.getTriangleList(triangleList1);
    draw_delaunay(img1, subdiv1, delaunay_color);
    // Draw points
    for( std::vector<cv::Point2f>::iterator iterator = points1.begin(); iterator != points1.end(); iterator++)
    {
        draw_point(img1, *iterator, points_color);
    }
    imshow("test", img1);
    waitKey(0);
    return 0;
}
