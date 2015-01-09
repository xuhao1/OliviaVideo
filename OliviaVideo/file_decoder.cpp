//
//  file_decoder.cpp
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include <stdio.h>
#include "decoder.h"

#define FILE_READING_BUFFER (1*1024*1024)

file_decoder::file_decoder(std::string file):
    h264_decoder(),path(file)
{
    fp = fopen(file.c_str(), "r");
}

AVPacket * file_decoder::build_avpkt(AVPacket *avpkt)
{
    static unsigned char buffer[1*1024*1024];
    static int readptr = 0;
    static int writeptr = 0;
    int len,toread;
    //AVPacket * avpkt = new AVPacket;
    //av_init_packet(avpkt);
    
    int nexthead;
    
    if (writeptr - readptr < 200 * 1024)
    {
        memmove(buffer, &buffer[readptr], writeptr - readptr);
        writeptr -= readptr;
        readptr = 0;
        toread = FILE_READING_BUFFER - writeptr;
        len = (int) fread(&buffer[writeptr], 1, toread, fp);
        writeptr += len;
    }
    
    nexthead = _find_head(&buffer[readptr], writeptr-readptr);
    
    if (nexthead == 0)
    {
        //printf("failed find next head...\n");
        nexthead = writeptr - readptr;
    }
    
    avpkt->size = nexthead;
    avpkt->data = &buffer[readptr];
    readptr += nexthead;
    return avpkt;
}

void file_decoder::run()
{
	printf("Starting with file %s",path.c_str());
	for (;;)
	{
		AVPacket * pkt = build_avpkt(&avpkt);
		if(pkt->size ==0)
			continue;
		decode_frame(pkt);
	}
}



