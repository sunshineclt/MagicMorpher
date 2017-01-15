//
//  main.cpp
//  MagicMorpher
//
//  Created by 陈乐天 on 2017/1/16.
//  Copyright © 2017年 陈乐天. All rights reserved.
//

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

int main() {
    // Set up frontal face detector & shape predictor(using the trained data from Internet)
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor sp;
    cout << "loading predictor data..." << endl;
    deserialize("/Users/cjjlt/programming/C++/MagicMorpher/shape_predictor_68_face_landmarks.dat") >> sp;
    
    image_window window;
    
    array2d<rgb_pixel> img;
    cout << "loading image..." << endl;
    load_image(img, "/Users/cjjlt/programming/C++/MagicMorpher/hillary_clinton.jpg");
    
    // Use the face detector to give us a list of bounding boxes around all the faces in the image
    std::vector<rectangle> dets = detector(img);
    cout << "Number of faces detected: " << dets.size() << endl;
    if (dets.size() > 1) {
        throw "More than one face are detected!";
    }
    rectangle det = dets[0];
    
    // Detect face landmarks
    std::vector<full_object_detection> shapes;
    full_object_detection shape = sp(img, det);
    cout << "number of parts: "<< shape.num_parts() << endl;
    for (unsigned int i = 0; i < shape.num_parts(); ++i) {
        cout << "pixel position of " << i << "th part: " << shape.part(i) << endl;
        dlib::point p = shape.part(i);
        // p 点的直径 3 参数为原点直径 rgb_pixel 颜色
        draw_solid_circle(img, p, 3, dlib::rgb_pixel(0, 255, 255));
    }
    shapes.push_back(shape);
    
    // View face landmarks
    window.clear_overlay();
    window.set_image(img);
    window.add_overlay(render_face_detections(shapes));
    
    dlib::array<array2d<rgb_pixel> > face_chips;
    extract_image_chips(img, get_face_chip_details(shapes), face_chips);
    //win_faces.set_image(tile_images(face_chips));
    
    
    
    cin.get();
    return 0;
}
