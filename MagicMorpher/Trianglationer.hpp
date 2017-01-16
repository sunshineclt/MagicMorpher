//
//  Trianglationer.hpp
//  MagicMorpher
//
//  Created by 陈乐天 on 2017/1/16.
//  Copyright © 2017年 陈乐天. All rights reserved.
//

#ifndef Trianglationer_hpp
#define Trianglationer_hpp

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

class Trianglationer {
public:
    Trianglationer(Mat img, std::vector<cv::Point2f> points);
    Trianglationer();
    void add_boundary();
    void display_points();
    void show_trianglation(Scalar delaunay_color, Scalar points_color);
    void do_trianglation();
private:
    void draw_delaunay(Mat & img, Scalar delaunay_color);
    void draw_points(Mat & img, Scalar color);
    std::vector<cv::Point2f> points;
    Mat img;
    Size size;
    Rect rect;
    Subdiv2D subdiv;
};

#endif /* Trianglationer_hpp */
