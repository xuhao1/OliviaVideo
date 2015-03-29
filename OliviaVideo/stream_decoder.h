//
//  stream_decoder.h
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#ifndef __OliviaVideo__stream_decoder__
#define __OliviaVideo__stream_decoder__

#include <stdio.h>

#include "decoder.h"

typedef unsigned char uchar;

class stream_decoder:public h264_decoder
{
    AVPacket * build_avpkt(uchar *,int);
public:
    int id = 0;
    stream_decoder(const char * _path):
        h264_decoder()
    {
        strcpy(path, _path);
        current = cv::imread(path);
    }
    
    void process(uchar*,int);
};
#endif /* defined(__OliviaVideo__stream_decoder__) */
