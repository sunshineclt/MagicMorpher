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
    
    
    
    //cin.get();
    return 0;
}
