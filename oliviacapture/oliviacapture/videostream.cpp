//
//  videostream.cpp
//  oliviacapture
//
//  Created by xuhao on 14/12/7.
//  Copyright (c) 2014年 xuhao. All rights reserved.
//

#include "videostream.h"

#include <iostream>
#include <string>
using namespace std;


#define BAD_NETWORK 0

videostream::videostream(std::string tar,int port)
{
    
    //create a UDP socket
    if ((socket_s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("Socket Create failed");
    }
    
    
    if ((socket_s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        printf("Failed");
    
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    if (inet_aton(tar.c_str(), &si_other.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
    }
    
    printf("build udp 2 %s via port %d\n",tar.c_str(),port);
}
int videostream::send_to_socket(const char*message ,int len)
{
#if BAD_NETWORK==1
    int k = rand()%1000;
    if (k<1)
    {
        printf("drop ");
        return 0;
    }
#endif
    const char * buf = message;
    int slen=sizeof(si_other);
    if (sendto(socket_s, buf, len
               , 0 , (sockaddr *)&si_other, slen)==-1)
    {
        printf("Failed:len %d\n",len);
        return -1;
    }
    
    return 0;
}


int videostream::write (const char* s,int len)
{
    static char data[1024*1024];
    memcpy(data,s,len*sizeof(char));
    frame_list fl = make_data_pack(data, len);
    if (fl.size() == 0)
        return -1;
    for (auto dp: fl)
    {
        int b_len = 0;
        char * buf  = dp->toBuf(b_len);
        if (b_len == 0)
            continue;
        send_to_socket(buf, b_len);
    }
    destory_data_pack_list(fl);
    return 0;
}

frame_list videostream::make_data_pack(const char*s, int len)
{
    if (len == 0)
        return frame_list(0);
    int num = 0;
    if (len % pack_size ==0)
    {
        num = len / pack_size;
    }
    else
    {
        num = len / pack_size + 1;
    }
    
    frame_list res(0);
    
    for (int i = 0; i<num; i++)
    {
        data_pack * df = new data_pack;
        df -> idf = (int32_t) id_f;
        df -> idp = (int32_t) id_p;
        df -> len_f =(int32_t) len;
        df -> num_f =(int32_t) num;
        df -> id_f_p =(int32_t)  i;
        df -> size_of_pack = pack_size;
        
        //memset(df->data, 0, pack_size);
        
        int cp_size = pack_size;
        if (i == num -1 )
            cp_size = len % pack_size;
        memcpy(df->data, s + i*pack_size, cp_size );
        
        id_p ++ ;
        res.push_back(df);
    }
    
    id_f ++;
    
    return res;
}

int videostream::destory_data_pack_list(frame_list&fl)
{
    for (auto a:fl)
    {
        if (a!=nullptr)
        {
            delete a;
        }
    }
    return 0;
}


#define SRV_IP "127.0.0.1"
/* diep(), #includes and #defines like in the server */
#define NPACK 10


void test_udp()
{
    videostream vs("127.0.0.1",7777);
}