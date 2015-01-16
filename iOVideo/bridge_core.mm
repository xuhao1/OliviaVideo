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

@implementation rec_core
    std::thread * th;
    stream_decoder * decoder;
    stream_reciver * rc;
- (id) init :(int) port
{
    printf("init core");
    if ( self=[super init] )
    {
        decoder =  new stream_decoder();
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
    return (char*)decoder->getCurrent().data;
}
@end