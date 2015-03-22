//
//  capcamera.h
//  oliviacapture
//
//  Created by xuhao on 9/15/14.
//  Copyright (c) 2014 xuhao. All rights reserved.
//
#ifndef __CAP_CAMERA_H__
#define __CAP_CAMERA_H__

#include "opencv2/opencv.hpp"
#include <thread>
#include <mutex>
#include <functional>

typedef std::function<cv::Mat()> cap_func;

class capcamera
{
private:
    cv::VideoCapture cap;
    std::mutex mtx;
    std::mutex mtx_stat;
    bool running = false;
    cv::Mat* currentframe = nullptr;
public:
    capcamera(int);
    capcamera(char*);
    void test();
    void capture();
    void end();
    std::thread capthread();
    cv::Mat current();
    cap_func capture_func();
};


#endif
