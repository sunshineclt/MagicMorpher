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

int main() {
    
    string shape_predictor_path = "/Users/cjjlt/programming/C++/MagicMorpher/shape_predictor_68_face_landmarks.dat";
    string pic1_path = "/Users/cjjlt/programming/C++/MagicMorpher/hillary_clinton.jpg";
    Mat img1 = imread(pic1_path);
    string pic2_path = "/Users/cjjlt/programming/C++/MagicMorpher/donald_trump.jpg";
    Mat img2 = imread(pic2_path);
    Scalar delaunay_color(255,255,255), points_color(0, 0, 255);
    
    FaceLandmarkGetter landmarkGetter1(shape_predictor_path, pic1_path);
    std::vector<cv::Point2f> points1 = landmarkGetter1.getFaceLandMark();
    Trianglationer trier1(img1, points1);
    trier1.add_boundary();
    trier1.display_points();
    trier1.do_trianglation();
    trier1.show_trianglation(delaunay_color, points_color);
    
    FaceLandmarkGetter landmarkGetter2(shape_predictor_path, pic2_path);
    std::vector<cv::Point2f> points2 = landmarkGetter2.getFaceLandMark();
    Trianglationer trier2(img2, points2);
    trier2.add_boundary();
    trier2.display_points();
    trier2.do_trianglation();
    trier2.show_trianglation(delaunay_color, points_color);
    
    return 0;
}
