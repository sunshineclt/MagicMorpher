//
//  FaceLandmarkGetter.hpp
//  MagicMorpher
//
//  Created by 陈乐天 on 2017/1/16.
//  Copyright © 2017年 陈乐天. All rights reserved.
//

#ifndef FaceLandmarkGetter_hpp
#define FaceLandmarkGetter_hpp

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace dlib;

class FaceLandmarkGetter {
public:
    FaceLandmarkGetter();
    FaceLandmarkGetter(string predictor_path, string picture_path);
    std::vector<cv::Point2f> getFaceLandMark();
private:
    string predictor_path, picture_path;
};

#endif /* FaceLandmarkGetter_hpp */
