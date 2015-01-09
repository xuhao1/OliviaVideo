//
//  stream_reciver.cpp
//  OliviaVideo
//
//  Created by Hao Xu on 15/1/9.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include "stream_reciver.h"
#define LIST_SIZE 256

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
    
    data_pack * tmp = new data_pack;
    tmp->fromBuf(buffer, len);
    
    return tmp;
}
 
int stream_reciver::test()
{
    while(1)
    {
        data_pack * tmp = rec();
        proc_frame(tmp);
        delete tmp;
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
    
    if (rec_list[ptr_frame]!=nullptr)
    {
        delete rec_list[ptr_frame];
        rec_list[ptr_frame] = nullptr;
    }
    
    rec_list[ptr_frame] = new frame_pkt;
    
    auto res = rec_list[ptr_frame];
    
    res->size = tmp->len_f;
    //res->data = new uchar(2*res->size);
    res->pkt_num = tmp->num_f;
    res->count =1;
    res->id = tmp->idf;
    
    memcpy(res->data, tmp->data, tmp->size_of_pack);
    
    return res;
}

void stream_reciver::complete_frame(data_pack * tmp)
{
    int frame_id = tmp->idf;
    int pkt_id = tmp->id_f_p;
    int id = 0;
    id = ptr_frame - (la_id_f - frame_id);
    
    if (id<0)
    {
        id += LIST_SIZE;
    }
    
    //printf("id :%d \n",id);
    
    frame_pkt * frame = rec_list[id];
    
    //printf("id :%3d frame :size:%3d\n",id,frame->id);
    
    memcpy(frame->data + pkt_id * tmp->size_of_pack,tmp->data,tmp->size_of_pack);
    
    frame->count ++;
    
    if(frame->count == frame->pkt_num)
    {
        callback(frame->data,frame->size,frame->id);
        printf("Complete id:%4d count :%3d frame_num:%3d size:%6d\n",frame->id,frame->count,frame->pkt_num,frame->size);
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
        complete_frame(tmp);
    }
}
