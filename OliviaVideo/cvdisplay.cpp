//
//  cvdisplay.cpp
//  OliviaVideo
//
//  Created by xuhao on 14/12/5.
//  Copyright (c) 2014年 xuhao. All rights reserved.
//

#include "cvdisplay.h"
#include "decoder.h"
#include <opencv2/opencv.hpp>
#include "stdio.h"
#include "stream_reciver.h"

int init_cv()
{
    return 0;
}

cv::Mat frame2mat(uchar * data[],int height,int width)
{
    cv::Mat YUV[3],res;
    YUV[0] = cv::Mat(height,width,CV_8UC1,data[0]);
    YUV[2] = cv::Mat(height/2,width/2,CV_8UC1,data[1]);
    YUV[1] = cv::Mat(height/2,width/2,CV_8UC1,data[2]);
    
    cv::resize(YUV[1],YUV[1],cv::Size(YUV[0].cols,YUV[0].rows));
    cv::resize(YUV[2],YUV[2],cv::Size(YUV[0].cols,YUV[0].rows));
    
    cv::merge(YUV, 3, res);
    cv::cvtColor(res, res, CV_YCrCb2BGR);
    
    return res;
 
}

int cv_show_frame(uchar * data[],int height,int width)
{
    static int num = 0;
    num++;
    cv::Mat res = frame2mat(data, height, width);
    cv::imshow("final", (cv::InputArray) res);
    cv::waitKey(30);
    return 0;
}