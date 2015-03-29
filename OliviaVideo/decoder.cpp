#include "decoder.h"
#include "cvdisplay.h"
#include <opencv2/opencv.hpp>
#include <vector>

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

#define INBUF_SIZE 40960


int h264_decoder::init()
{
    avcodec_register_all();
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        return -1;
    }
    
    avctx = avcodec_alloc_context3(codec);
    if (!avctx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        return -1;
    }
    
    if(codec->capabilities&CODEC_CAP_TRUNCATED)
        avctx->flags|= CODEC_FLAG_TRUNCATED;

    av_init_packet(&avpkt);
    
    
    /* we do not send complete frames */
    
    /* For some codecs, such as msmpeg4 and mpeg4, width and height
     MUST be initialized there because this information is not
     available in the bitstream. */
    
    /* open it */
    if (avcodec_open2(avctx, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }
    
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        return -1;
    }
    
    frame_count = 0;
    
    return 0;
}
h264_decoder::h264_decoder()
{
    init();
    strcpy(path, "/Users/xuhao/data/a.jpg");
    current = cv::imread(path);
}
cv::Mat h264_decoder::getCurrent()
{
    mtx.lock();
    cv::Mat frame = current.clone();
    mtx.unlock();
    return frame;
}
cv::Mat h264_decoder::getCurrentRGB()
{
    mtx.lock();
    cv::Mat frame = current.clone();
    mtx.unlock();
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    return frame;
}
int h264_decoder::_find_head(uchar *buffer, int len)
{
    int i = 0;
    
    for (i=512;i<len;i++)
    {
        if (buffer[i] == 0
            && buffer[i+1] == 0
            && buffer[i+2] == 0
            && buffer[i+3] == 1)
            break;
    }
    if (i == len)
        return 0;
    if (i == 512)
        return 0;
    return i;
}
cv::Mat h264_decoder::convert2mat(uchar ** data, int height, int width)
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

cv::Mat h264_decoder::decode_frame(AVPacket * pkt)
{
    int got_frame;
    
    int len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);
    
    if (len < 0) {
        fprintf(stderr, "Error while decoding frame \n");
        //   exit(1);
        return cv::Mat();
    }
    if (got_frame) {
        // the picture is allocated by the decoder. no need to
         // free it
        mtx.lock();
        current = convert2mat((uchar **)frame->data, (int) avctx->height,(int) avctx->width);
        mtx.unlock();
    }
    pkt->size -= len;
    pkt->data += len;
    return cv::Mat();
}


void test_decoder()
{
	file_decoder fr("/Users/xuhao/data/a.mov");
	fr.run();
}
