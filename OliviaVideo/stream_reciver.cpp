//
//  stream_reciver.cpp
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include "stream_reciver.h"
#include "sys/time.h"



#define LIST_SIZE 1024
#define MAX_WAIT 200000

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

stream_reciver::stream_reciver(int port)
{
    
    frame_callback f = [&](uchar* data,int size,int id)
    {
        printf("id:%d",size);
    };
    
    
    new (this)stream_reciver(port,f);
}

stream_reciver::stream_reciver(int port,frame_callback f):
    callback(f), 
    rec_list(LIST_SIZE),ptr_frame(-1),la_id_f(-1)
{
    addr_len = sizeof(struct sockaddr_in);
    
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror ("socket");
        exit(1);
    }
    /*填写sockaddr_in 结构*/
    bzero ( &addr, sizeof(addr) );
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=htonl(INADDR_ANY) ;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))<0){
        perror("connect");  
        exit(1);
    }
    
}


data_pack * stream_reciver::rec()
{
    
    
    bzero(buffer,sizeof(buffer));
    
    int len =(int) recvfrom(sockfd,(void*)buffer,sizeof(buffer), 0 , (struct sockaddr *)&addr ,(socklen_t *)&addr_len);
    //printf("gotta %d\n",len);
    data_pack * tmp = new data_pack;
    tmp->fromBuf(buffer, len);
    
    return tmp;
}
 
int stream_reciver::test()
{
    static long la = getCurrentTime();
    while(1)
    {
        data_pack * tmp = rec();
        if (getCurrentTime() - la > 1000000)
        {
            printf("timeout");
            reset(tmp);
        }
        proc_frame(tmp);
        delete tmp;
        la = getCurrentTime();
    }
    return 0;
}

frame_pkt * stream_reciver::new_pkt_frame(data_pack * tmp)
{
    la_id_f = tmp->idf;
    ptr_frame ++;
    if (ptr_frame >= rec_list.size())
    {
        ptr_frame = 0;
    }
    
    auto res = &rec_list[ptr_frame];
    
    res->size = tmp->len_f;
    
    res->pkt_num = tmp->num_f;
    res->count =1;
    res->id = tmp->idf;
    res->rec_time = getCurrentTime();
    res->pubed = false;
    
    memcpy(res->data, tmp->data, tmp->size_of_pack);
    
    
    return res;
}

void stream_reciver::frame_finish_callback(frame_pkt * frame,int this_ptr)
{
    
    
    long time = getCurrentTime();
    
    int i = last_pub_ptr;
    if (i>this_ptr)
    {
        i -= LIST_SIZE;
    }
    while  (i <= this_ptr)
    {
        i ++;
        
        int ptr = i;
        
        if (i<0)
        {
            ptr +=LIST_SIZE;
        }
        
        frame_pkt * fm = &rec_list[ptr];
        if (
            (time - fm->rec_time > MAX_WAIT || fm->count == fm->pkt_num)
            && !fm->pubed &&fm->id !=0 )
        {
            if (fm->count!=fm->pkt_num)
                printf("frame ptr:%4d id :%4d delay:%3ld count:%3d/%3d\n",ptr,fm->id,(time - fm->rec_time)/1000,fm->count,fm->pkt_num);
            last_pub = fm->id;
            last_pub_ptr = ptr ;
            fm->pubed = true;
            callback(fm->data,fm->size,fm->id);
        }
    }
}

void stream_reciver::resume_frame(data_pack * tmp)
{
    int frame_id = tmp->idf;
    int pkt_id = tmp->id_f_p;
    int this_ptr = 0;
    this_ptr = ptr_frame - (la_id_f - frame_id);
    
    if (this_ptr<0)
    {
        this_ptr += LIST_SIZE;
    }
    
    
    frame_pkt * frame = &rec_list[this_ptr];
    
    memcpy(frame->data + pkt_id * tmp->size_of_pack,tmp->data,tmp->size_of_pack);
    
    frame->count ++;
    
    
    if(frame->count == frame->pkt_num && frame_id > last_pub)
    {
        frame_finish_callback(frame,this_ptr);
    }
}

void stream_reciver::proc_frame(data_pack * tmp)
{
    if ( tmp->idf > la_id_f )
    {
        new_pkt_frame(tmp);
    }
    else
    {
        resume_frame(tmp);
    }
}

void stream_reciver::reset(data_pack * tmp)
{
    la_id_f = tmp->idf;
    last_pub = tmp->idf;
    last_pub_ptr = 0;
    ptr_frame = 0;
}
