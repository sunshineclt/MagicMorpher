//
//  Trianglationer.cpp
//  MagicMorpher
//
//  Created by 陈乐天 on 2017/1/16.
//  Copyright © 2017年 陈乐天. All rights reserved.
//

#include "Trianglationer.hpp"

using namespace std;
using namespace cv;

/// no parameter constructor
Trianglationer::Trianglationer() {}

/// constructor
Trianglationer::Trianglationer(Mat img, std::vector<cv::Point2f> points) {
    this->img = img;
    this->points = points;
    this->size = img.size();
    this->rect = Rect(0, 0, size.width, size.height);
}

/// add corner points(4 corner points and 4 halfway points) to this->points
void Trianglationer::add_boundary() {
    int x = size.width;
    int y = size.height;
    points.push_back(Point2f(0, 0));
    points.push_back(Point2f(0, y / 2));
    points.push_back(Point2f(0, y - 1));
    points.push_back(Point2f(x / 2, 0));
    points.push_back(Point2f(x / 2, y - 1));
    points.push_back(Point2f(x - 1, 0));
    points.push_back(Point2f(x - 1, y / 2));
    points.push_back(Point2f(x - 1, y - 1));
}

/**
 display points
 - note: for debug
 */
void Trianglationer::display_points() {
    for (unsigned int i = 0; i < points.size(); ++i) {
        cout << i << "th part: " << points[i] << endl;
    }
}

/**
 do trianglation work
 result is in this->subdiv
 use `get_trianglation_result()` or `get_trianglation_index_result()` to get the result of trianglation
 */
void Trianglationer::do_trianglation() {
    subdiv = Subdiv2D(rect);
    for (std::vector<cv::Point2f>::iterator iterator = points.begin(); iterator != points.end(); ++iterator) {
        subdiv.insert(*iterator);
    }
}

/// get the trianglation result using the format of coordinate
std::vector<std::vector<Point2f> > Trianglationer::get_trianglation_result() {
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point2f> point(3);
    std::vector<std::vector<Point2f> > result;
    
    for(size_t i = 0; i < triangleList.size(); i++) {
        Vec6f t = triangleList[i];
        point[0] = Point2f(t[0], t[1]);
        point[1] = Point2f(t[2], t[3]);
        point[2] = Point2f(t[4], t[5]);
        
        if (rect.contains(point[0]) && rect.contains(point[1]) && rect.contains(point[2])) {
            result.push_back(points);
        }
    }
    return result;
}

/// get the trianglation result using the format of index
std::vector<std::vector<int> > Trianglationer::get_trianglation_index_result() {
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point2f> point(3);
    std::vector<std::vector<int> > result;
    
    for(size_t i = 0; i < triangleList.size(); i++) {
        Vec6f t = triangleList[i];
        point[0] = Point2f(t[0], t[1]);
        point[1] = Point2f(t[2], t[3]);
        point[2] = Point2f(t[4], t[5]);
        
        if (rect.contains(point[0]) && rect.contains(point[1]) && rect.contains(point[2])) {
            int x1, x2, x3;
            for (unsigned int i1 = 0; i1 < points.size(); ++i1) {
                if (points[i1].x == point[0].x && points[i1].y == point[0].y) {
                    x1 = i1;
                    break;
                }
            }
            for (unsigned int i2 = 0; i2 < points.size(); ++i2) {
                if (points[i2].x == point[1].x && points[i2].y == point[1].y) {
                    x2 = i2;
                    break;
                }
            }
            for (unsigned int i3 = 0; i3 < points.size(); ++i3) {
                if (points[i3].x == point[2].x && points[i3].y == point[2].y) {
                    x3 = i3;
                    break;
                }
            }
            std::vector<int> po;
            po.push_back(x1);
            po.push_back(x2);
            po.push_back(x3);
            result.push_back(po);
        }
    }
    return result;
}

/**
 show trianglation
 - note: for demo

 @param delaunay_color the color for lines
 @param points_color the color for points
 */
void Trianglationer::show_trianglation(Scalar delaunay_color, Scalar points_color) {
    Mat img_show = img.clone();
    draw_delaunay(img_show, delaunay_color);
    draw_points(img_show, points_color);
    imshow("test", img_show / 255.0);
    cout << "focus on the image and press any key to continue..." << endl;
    waitKey(0);
}

/// used in `show_trianglation` to draw lines
void Trianglationer::draw_delaunay(Mat & img, Scalar delaunay_color) {
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point> point(3);
    
    for(size_t i = 0; i < triangleList.size(); i++) {
        Vec6f t = triangleList[i];
        point[0] = Point(cvRound(t[0]), cvRound(t[1]));
        point[1] = Point(cvRound(t[2]), cvRound(t[3]));
        point[2] = Point(cvRound(t[4]), cvRound(t[5]));
        
        // Draw rectangles completely inside the image.
        if (rect.contains(point[0]) && rect.contains(point[1]) && rect.contains(point[2])) {
            line(img, point[0], point[1], delaunay_color, 1, CV_AA, 0);
            line(img, point[1], point[2], delaunay_color, 1, CV_AA, 0);
            line(img, point[2], point[0], delaunay_color, 1, CV_AA, 0);
        }
    }
}

/// used in `show_trianglation` to draw points
void Trianglationer::draw_points(Mat & img, Scalar color) {
    for(std::vector<cv::Point2f>::iterator iterator = points.begin(); iterator != points.end(); iterator++) {
        circle(img, *iterator, 2, color, CV_FILLED, CV_AA, 0);
    }
}
