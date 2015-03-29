//
//  stream_reciver.h
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#ifndef __OliviaVideo__stream_reciver__
#define __OliviaVideo__stream_reciver__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <functional>

#include <vector>

#include <stdio.h>

typedef unsigned char uchar;
#define PACK_SIZE 2048
typedef std::function<void(uchar*,int,int)> frame_callback;

struct data_pack
{
    int32_t idp,idf;
    int32_t len_f,num_f,id_f_p,size_of_pack;
    char data[PACK_SIZE] = {0};
    void fromBuf(char * buf ,int len)
    {
        if (data == nullptr)
        {
            len = 0;
        }
        
        memcpy(&idp,buf, 4);
        memcpy(&idf,buf+4, 4);
        
        memcpy(&len_f,buf+8, 4);
        memcpy(&num_f,buf+12, 4);
        
        memcpy(&id_f_p,buf+16, 4);
        memcpy(&size_of_pack,buf+20, 4);
        
        memcpy(data,buf+24, size_of_pack);
    }
};

struct frame_pkt
{
    uchar data[128*1024];
    int size;
    int count = 0;
    int pkt_num = 0;
    int id;
    long rec_time = 0;
    bool pubed = false;
};


class stream_reciver
{
    int sockfd,len;
    struct sockaddr_in addr;
    char buffer[1024*1024];
    int addr_len;
    
    int ptr_frame;
    int la_id_f;
    int last_pub=0,last_pub_ptr=0;
    void proc_frame(data_pack *tmp);
    frame_pkt *  new_pkt_frame(data_pack *);
    frame_callback callback;
    
    void frame_finish_callback(frame_pkt*,int);
    
    void reset(data_pack*);
    
public:
    
    stream_reciver(int port);
    stream_reciver(int port,frame_callback f);
    
    std::vector<frame_pkt> rec_list;
    data_pack* rec();
    void resume_frame(data_pack*);
    int test();
    
    int process();
   
};

#endif /* defined(__OliviaVideo__stream_reciver__) */
