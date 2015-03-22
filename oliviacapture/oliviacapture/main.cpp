//
//  main.cpp
//  oliviacapture
//
//  Created by xuhao on 9/15/14.
//  Copyright (c) 2014 xuhao. All rights reserved.
//

#include "headers.h"

using namespace std;

typedef function<int(char*,int)> write_func;

thread start_encoder(cap_func capture,write_func fwrite )
{
       thread t_encoder([&]{
        CV_encoder cve(capture,fwrite);
        cve.run();
    });
    return t_encoder;
}

void cmd()
{
    
}
void start()
{
    ofstream * log= new ofstream("/Users/xuhao/data/abc.h264");
    
    //videostream *udp = new videostream("121.40.33.249",7890);
    videostream *udp = new videostream("127.0.0.1",7890);
    
    thread t[4];
    
    function<int(char*,int)> fwrite = [&](char* buf,int len)
    {
        int ran = rand() % len;
        int length = rand() % 1;
        length = length % (len - ran);
        
        memset(buf+ran, 0, length*sizeof(char));
        
        log->write(buf, len);
        return 0;
    };
    
    auto udp_write = [&](char* buf,int len)
    {
        return udp -> write(buf, len);
    };
    
    
    //capcamera cap("/Users/xuhao/data/test.avi");
    capcamera cap(0);
    
    t[0] = cap.capthread();
    //t[2] = start_encoder(cap.capture_func(), udp_write);
    
    map<string,function<void(thread*)>> func_list;
    
    CV_encoder cve(cap.capture_func(),udp_write);
    cve.run();
    
    string str;
    vector<thread*> thread_list;
    int k = 0;
    cap_func cap0 = cap.capture_func();
    
    while (true)
    {
        cout<<"~>";
        getline(cin,str);
        if (func_list.find(str) == func_list.end())
        {
            printf("No this cmd\n");
            continue;
        }
        
        auto func_t = func_list[str];
        thread * a;
        func_t(a);
        thread_list.push_back(a);
        k = 0;
    }
    
}

int main(int argc, const char * argv[])
{
    // insert code here...
    start();
    return 0;
}
