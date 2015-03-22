//
//  display.cpp
//  oliviacapture
//
//  Created by xuhao on 14/12/7.
//  Copyright (c) 2014年 xuhao. All rights reserved.
//

#include "display.h"

int display(cv::Mat frame)
{
    cv::imshow("Frame",frame);
    cv::waitKey(30);
}