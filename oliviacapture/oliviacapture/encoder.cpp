//
//  encoder.cpp
//  oliviacapture
//
//  Created by xuhao on 9/15/14.
//  Copyright (c) 2014 xuhao. All rights reserved.
//

#include "encoder.h"
#include "stdio.h"
#include <string>
#include <functional>
#include <ostream>
#include <fstream>
#include "sys/time.h"

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}

int codec_id =AV_CODEC_ID_H264;

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}
int encoder::init()
{
    avcodec_register_all();
    height = 720;
    width = 1280;
    
    int  ret;
    //uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    codec = avcodec_find_encoder((enum AVCodecID)codec_id);
    //codec = avcodec_find_encoder_by_name("h264_vda");
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        return -1;
    }
    
    reset();
    
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        return 1;
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_image_alloc((uint8_t **)frame->data,(int*) frame->linesize, c->width, c->height,
                             c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        return -1;
    }
    return 0;
}

int encoder::reset()
{
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        return -1;
    }
    
    av_opt_set(c->priv_data, "preset", "superfast", 0);
    av_opt_set(c->priv_data, "tune", "zerolatency", 0);
    /* put sample parameters */
    c->bit_rate = bit_rate;
    /* resolution must be a multiple of two */
    c->width = width;
    c->height = height;
    /* frames per second */
    c->time_base = (AVRational){1,25};
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    
    
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }
    
    return 0;
}

uchar ** encoder::get_data()
{
    uchar **data = new uchar*[3];
    for (int i = 0;i<3;i++ )
    {
        data[i] = new uchar[width*height];
    }
    int x , y ;
    int i = 0;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            data[0][y * width + x] = x + y + i * 3;
        }
    }
    
    /* Cb and Cr */
    for (y = 0; y < height/2; y++) {
        for (x = 0; x < width/2; x++) {
            data[1][y * width + x] = 128 + y + i * 2;
            data[2][y * width + x] = 64 + x + i * 5;
        }
    }
    return  data;
}
int encoder::write()
{
    ki++;
    
    
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;    // packet data will be allocated by the encoder
    pkt.size = 0;
    
    /* prepare a dummy image */
    /* Y */
    uchar ** data  = get_data();
    frame->data[0] = data[0];
    frame->data[1] = data[1];
    frame->data[2] = data[2];
    frame->pts = ki;
    
    /* encode the image */
    int got_output;
    int ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
    if (ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        return -1;
    }
    
    static long count_size = 0;
    count_size += pkt.size;
    
    if (got_output)
    {
        fwrite((char *)pkt.data, pkt.size);
        av_free_packet(&pkt);
    }
    
    
    return 0;
}
void encoder::run()
{
    running = true;

    while(running)
    {
        pause_lock.lock();
        write();
        pause_lock.unlock();
    }
    
    avcodec_close(c);
}
void encoder::stop()
{
    running = false;
}

CV_encoder::CV_encoder(std::function<cv::Mat()> getFrame,std::function<int(char*,int)> fwrite,float rate_m)
{
    this->bit_rate = rate_m * ONE_M;

    while (init()!=0)
        fprintf(stderr, "init failed ,try again");

    printf("init encoder with bit rate %3fM successful\n",rate_m);
    this->getFrame = getFrame;
    this->fwrite = fwrite;

    pause_lock.lock();
    //GETFRAME SHOULD BE RGB
}

uchar ** CV_encoder::get_data()
{
    static int count = 0;
    count ++;
    
    cv::Mat frame = this->getFrame();
    
    
    if (frame.cols == 0)
    {
        return nullptr;
    }
    
    if(frame.cols != width && frame.rows !=height&&frame.cols!=0)
        cv::resize(frame, frame, cv::Size(width,height));
    cv::cvtColor(frame, frame, CV_BGR2YCrCb);
    cv::Mat YUV[3];//Y Cb Cr
    split(frame,YUV);
    cv::resize(YUV[1], YUV[1], cv::Size(frame.cols/2,frame.rows/2));
    cv::resize(YUV[2], YUV[2], cv::Size(frame.cols/2,frame.rows/2));
    uchar ** data = new uchar*[3];
    data[0] = YUV[0].data;
    data[1] = YUV[2].data;
    data[2] = YUV[1].data;
    return data;
}
