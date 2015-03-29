//
//  brigde_core.cpp
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include "bridge_core.h"
#include <functional>
#include <thread>
#include "stream_decoder.h"
#include "stream_reciver.h"
#include <opencv2/opencv.hpp>

NSImage * UIImageFromCVMat(cv::Mat cvMat)
{
    
    NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
    CGColorSpaceRef colorSpace;
    
    if (cvMat.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(cvMat.cols,                                 //width
                                        cvMat.rows,                                 //height
                                        8,                                          //bits per component
                                        8 * cvMat.elemSize(),                       //bits per pixel
                                        cvMat.step[0],                            //bytesPerRow
                                        colorSpace,                                 //colorspace
                                        kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
                                        provider,                                   //CGDataProviderRef
                                        NULL,                                       //decode
                                        false,                                      //should interpolate
                                        kCGRenderingIntentDefault                   //intent
                                        );
    
    
    // Getting UIImage from CGImage
    NSSize   nsize;
    nsize.width = cvMat.cols;
    nsize.height = cvMat.rows;
    
    NSImage *finalImage = [[NSImage alloc] initWithCGImage:imageRef size:nsize];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    return finalImage;
}

@implementation rec_core
    std::thread * th;
    stream_decoder * decoder;
    stream_reciver * rc;
- (id) init :(int) port
{
    
    NSString * pat = [[NSBundle mainBundle] pathForResource:@"hold" ofType:@"jpg" inDirectory:@"imgs" ];
    
   const char * pa = [pat UTF8String];
    
    if ( self=[super init] )
    {
        decoder =  new stream_decoder(pa);
        auto f = [&](uchar* data,int len,int id)
        {
            decoder->process(data, len);
            decoder->id = id;
        };
        
        rc = new stream_reciver(port,f);
        
        auto f_rc = [&]
        {
            printf("staring listen\n");
            rc->test();
        };
        
        th =new std::thread(f_rc);
    }
    
    printf("init successful!\n");
    return self;
}

- (char*)getCurrent
{
    return (char*)decoder->getCurrentRGB().data;
}
-(NSImage *) getCurrentImg
{
    return UIImageFromCVMat(decoder->getCurrentRGB());
}
@end