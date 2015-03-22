//
//  capcamera.cpp
//  oliviacapture
//
//  Created by xuhao on 9/15/14.
//  Copyright (c) 2014 xuhao. All rights reserved.
//

#include <stdio.h>
#include "capcamera.h"
#include "opencv2/opencv.hpp"
#include <mutex>
#include <thread>
#include "encoder.h"
#include <fstream>

using namespace cv;
capcamera::capcamera(int num):
    cap(num)
{
    if(!cap.isOpened())  // check if we succeeded
    {
        printf("Cannot Open The Camera");
    }
    this->running = true;
}
capcamera::capcamera(char * path):
    cap(path)
{
    if(!cap.isOpened())  // check if we succeeded
    {
        printf("Cannot Open The Camera");
    }
    this->running = true;
}
void capcamera::capture()
{
    mtx.lock();
    if(currentframe == nullptr)
    {
        currentframe = new cv::Mat;
    }
    cv::Mat tmp;
    cap>> * currentframe;
    if (currentframe->rows == 0)
        printf("no img");
    mtx.unlock();
    
}
void capcamera::end()
{
    mtx_stat.lock();
    this->running = false;
    this->cap.release();
    mtx_stat.unlock();
}
std::thread capcamera::capthread()
{
    std:: thread t_cap([&]{
        while(this->running)
        {
            this->mtx_stat.lock();
            this->capture();
            mtx_stat.unlock();
        }
    });
    return t_cap;
}

Mat capcamera::current()
{
    if (currentframe == nullptr)
    {
        capture();
    }
    mtx.lock();
    Mat frame;
    frame = currentframe->clone();
    mtx.unlock();
    return frame;
}


cap_func capcamera::capture_func()
{
    
    std::function<cv::Mat()> cap_f = [&]()
    {
        cv::Mat frame = this->current();
        return frame;
    };
    return cap_f;
}
