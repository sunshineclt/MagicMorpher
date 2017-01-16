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

/**
 This class is used to do trianglation(Delaunay Triangulation)
 */
class Trianglationer {
public:
    /// no parameter constructor
    Trianglationer();
    /// constructor
    Trianglationer(Mat img, std::vector<cv::Point2f> points);
    /// add corner points(4 corner points and 4 halfway points) to this->points
    void add_boundary();
    /// display points(for debug)
    void display_points();
    /// show trianglation(for demo)
    void show_trianglation(Scalar delaunay_color, Scalar points_color);
    /// do trianglation work and result is in this->subdiv
    void do_trianglation();
    /// get the trianglation result using the format of coordinate
    std::vector<std::vector<Point2f> > get_trianglation_result();
    /// get the trianglation result using the format of index
    std::vector<std::vector<int> > get_trianglation_index_result();

    std::vector<cv::Point2f> points;
private:
    /// used in show_trianglation to draw lines
    void draw_delaunay(Mat & img, Scalar delaunay_color);
    /// used in show_trianglation to draw points
    void draw_points(Mat & img, Scalar color);
    Mat img;
    Size size;
    Rect rect;
    Subdiv2D subdiv;
};

#endif /* Trianglationer_hpp */
