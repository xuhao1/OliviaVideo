//
//  stream_decoder.cpp
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include "stream_decoder.h"

void stream_decoder::process(uchar* data, int len)
{
    AVPacket * pkt =build_avpkt(data, len);
    if(pkt->size == 0)
        return;
    decode_frame(pkt);
}

AVPacket * stream_decoder::build_avpkt(uchar *data, int len)
{
    avpkt.size = len;
    avpkt.data = data;
    return &avpkt;
}