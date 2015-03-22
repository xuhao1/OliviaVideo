//
//  videostream.h
//  oliviacapture
//
//  Created by xuhao on 14/12/7.
//  Copyright (c) 2014年 xuhao. All rights reserved.
//

#ifndef __oliviacapture__videostream__
#define __oliviacapture__videostream__

#include <stdio.h>
#include <string>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<errno.h>
#include <unistd.h>
#include <ostream>
#include <vector>
#include <stdint.h>
#define BUFLEN 512
#define PACK_SIZE 256
#define EXTRA_LEN 24

using namespace std;
struct data_pack
{
    int32_t idp,idf;
    int32_t len_f,num_f,id_f_p,size_of_pack;
    char data[PACK_SIZE] = {0};
    char buf[PACK_SIZE+EXTRA_LEN];
    char * toBuf(int &len)
    {
        if (data == nullptr)
        {
            len = 0;
            return nullptr;
        }
        
        len = EXTRA_LEN+size_of_pack;
        
        memcpy(buf, &idp, 4);
        memcpy(buf+4, &idf, 4);
        
        memcpy(buf+8, &len_f, 4);
        memcpy(buf+12, &num_f, 4);
        
        memcpy(buf+16, &id_f_p, 4);
        memcpy(buf+20, &size_of_pack, 4);
        
        memcpy(buf+24, data, size_of_pack);
        return buf;
    }
};

typedef std::vector<data_pack*> frame_list;

class videostream
{
    int socket_s;
    long id_f=0,id_p=0;
    int pack_size = PACK_SIZE;
    int send_to_socket(const char*message,int len);
    struct sockaddr_in  si_other;
public:
    videostream(std::string,int);
    int send(const char * buf);
    int write(const char* s,int len);
    frame_list make_data_pack(const char * s,int len);
    int destory_data_pack_list(frame_list&);
};


void test_udp();

int udp_main();
#endif /* defined(__oliviacapture__videostream__) */
