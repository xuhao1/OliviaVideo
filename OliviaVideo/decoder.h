#ifndef DECODER_H
#define DECODER_H

#include <mutex>
#include "opencv2/opencv.hpp"


typedef unsigned char uchar;
extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
};

void decode();

class h264_decoder
{
protected:
    int _find_head(uchar*buffer,int len);
    AVPacket * build_avpkt();
    std::mutex mtx;
    
    int init();
    
    cv::Mat decode_frame(AVPacket * pkt);
    cv::Mat convert2mat(uchar **data,int height,int width);
    
    
    
    AVCodec *codec;
    AVCodecContext *avctx= NULL;
    int frame_count;
    AVFrame *frame;
    AVPacket avpkt;
    h264_decoder();
    
public:
    cv::Mat getCurrent();
    cv::Mat current;
};


class file_decoder:public h264_decoder
{
    AVPacket *build_avpkt(AVPacket *avpkt);
    FILE * fp;
	std::string path;
public:
    file_decoder(std::string file);
    void run();
    
};

#endif // DECODER_H
