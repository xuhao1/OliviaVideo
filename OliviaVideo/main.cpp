//
//  main.cpp
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include <stdio.h>
#include "stream_reciver.h"
#include "decoder.h"
#include "stream_decoder.h"
#include <functional>
#include <thread>

int test()
{
    stream_decoder decoder;
    auto f = [&](uchar* data,int len,int id)
    {
        //printf("call decoder");
        decoder.process(data, len);
        decoder.id = id;
    };
    
    stream_reciver rc(7777,f);
    
    auto show= [&]
    {
        cv::namedWindow("current");
        while (true)
        {
            char text[100];
            sprintf(text, "current:%3d",decoder.id);
            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 2;
            int thickness = 3;
            cv::Mat frame =decoder.getCurrent();
            int baseline=0;
            cv::Size textSize = cv::getTextSize(text, fontFace,
                                        fontScale, thickness, &baseline);
            cv::Point textOrg((frame.cols - textSize.width)/2,
                          (frame.rows + textSize.height)/2);
            
            cv::putText(frame, text, textOrg, fontFace, fontScale,
                        cv::Scalar::all(255), thickness, 8);
            
           // cv::imshow("current", decoder.current);
            //cv::waitKey(30);
        }
    };
    
    auto f_rc = [&]
    {
        rc.test();
    };
    
    std :: thread t_rc(f_rc);
    show();
    t_rc.join();
    return 0;
}

int test_networking()
{
    stream_reciver rc(7777);
    rc.test();
    return 0;
}

int main()
{
    printf("starting recv\n");
    test();
    //test_networking();
//    test_decoder();
    //decode();
}