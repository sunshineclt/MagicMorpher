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

Trianglationer::Trianglationer() {}

Trianglationer::Trianglationer(Mat img, std::vector<cv::Point2f> points) {
    this->img = img;
    this->points = points;
    this->size = img.size();
    this->rect = Rect(0, 0, size.width, size.height);
}

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

void Trianglationer::display_points() {
    for (unsigned int i = 0; i < points.size(); ++i) {
        cout << i << "th part: " << points[i] << endl;
    }
}

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

void Trianglationer::draw_points(Mat & img, Scalar color) {
    for(std::vector<cv::Point2f>::iterator iterator = points.begin(); iterator != points.end(); iterator++) {
        circle(img, *iterator, 2, color, CV_FILLED, CV_AA, 0);
    }
}

void Trianglationer::do_trianglation() {
    subdiv = Subdiv2D(rect);
    for (std::vector<cv::Point2f>::iterator iterator = points.begin(); iterator != points.end(); ++iterator) {
        subdiv.insert(*iterator);
    }
}

void Trianglationer::show_trianglation(Scalar delaunay_color, Scalar points_color) {
    Mat img_show = img.clone();
    draw_delaunay(img_show, delaunay_color);
    draw_points(img_show, points_color);
    imshow("test", img_show);
    waitKey(0);
}
