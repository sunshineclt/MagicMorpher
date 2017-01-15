//
//  FaceLandmarkGetter.cpp
//  MagicMorpher
//
//  Created by 陈乐天 on 2017/1/16.
//  Copyright © 2017年 陈乐天. All rights reserved.
//

#include "FaceLandmarkGetter.hpp"

using namespace std;
using namespace dlib;
using namespace cv;

FaceLandmarkGetter::FaceLandmarkGetter() {
};

FaceLandmarkGetter::FaceLandmarkGetter(string predictor_path, string picture_path) {
    this->predictor_path = predictor_path;
    this->picture_path = picture_path;
}


std::vector<cv::Point2f> FaceLandmarkGetter::getFaceLandMark() {
    // Set up frontal face detector & shape predictor(using the trained data from Internet)
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor sp;
    cout << "loading predictor data..." << endl;
    deserialize(predictor_path) >> sp;
    
    image_window window;
    
    array2d<rgb_pixel> img;
    cout << "loading image..." << endl;
    load_image(img, picture_path);
    
    // Use the face detector to give us a list of bounding boxes around all the faces in the image
    std::vector<dlib::rectangle> dets = detector(img);
    cout << "Number of faces detected: " << dets.size() << endl;
    if (dets.size() > 1) {
        throw "More than one face are detected!";
    }
    dlib::rectangle det = dets[0];
    
    // Detect face landmarks
    std::vector<full_object_detection> shapes;
    full_object_detection shape = sp(img, det);
    std::vector<Point2f> points;
    cout << "number of parts: "<< shape.num_parts() << endl;
    for (unsigned int i = 0; i < shape.num_parts(); ++i) {
        dlib::point p = shape.part(i);
        points.push_back(Point2f(p.x(), p.y()));
        draw_solid_circle(img, p, 3, dlib::rgb_pixel(0, 255, 255));
    }
    shapes.push_back(shape);
    
    // View face landmarks
//    window.clear_overlay();
//    window.set_image(img);
//    window.add_overlay(render_face_detections(shapes));
    
//    dlib::array<array2d<rgb_pixel> > face_chips;
//    extract_image_chips(img, get_face_chip_details(shapes), face_chips);
    //win_faces.set_image(tile_images(face_chips));
    
//    cin.get();
    
    return points;
};
