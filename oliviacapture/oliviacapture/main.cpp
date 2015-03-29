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

thread t[4];
CV_encoder * en[3] = {0};

thread start_encoder(cap_func capture,write_func fwrite ,float rate,
        int no
)
{
    CV_encoder * cve;

    thread t_encoder([&](float _rate,int _no){
        printf("try to init encoder no:%d \n",_no);
        cve = new CV_encoder(capture,fwrite,_rate);
        printf("init cve no:%d successful\n",_no);
        en[_no] = cve;
        cve->run();
    },rate,no);
    return t_encoder;
}

void start() {
    ofstream *log = new ofstream("/Users/xuhao/data/abc.h264");

    //videostream *udp = new videostream("121.40.33.249",7890);
    std::string ip = "100.65.0.10";
    videostream *udp = new videostream(ip, 7890);


    function<int(char *, int)> fwrite = [&](char *buf, int len) {
        /*
        int ran = rand() % len;
        int length = rand() % 1;
        length = length % (len - ran);

        memset(buf+ran, 0, length*sizeof(char));
        */
        log->write(buf, len);
        return 0;
    };

    auto udp_write = [&](char *buf, int len) {
        return udp->write(buf, len);
    };


    capcamera cap(0);

    t[0] = cap.capthread();

    map<string, function<void(thread *)>> func_list;

    t[1] = start_encoder(cap.capture_func(), udp_write, 4.0f, 0);
    t[2] = start_encoder(cap.capture_func(), udp_write, 2.0f, 1);
    t[3] = start_encoder(cap.capture_func(), udp_write, 0.5f, 2);

    while (en[0] == nullptr);

    printf("startat en 0\n");
    en[0]->start();

    while (true)
    {
        sleep(1);
    }
    /*
    while (true)
    {
        printf("switching to encode 0\n");
        en[0]->start();
        sleep(10);
        printf("switching to encode 1\n");
        en[0]->stop();
        en[1]->start();
        sleep(10);
        printf("switching to encode 2\n");
        en[1]->stop();
        en[2]->start();
        sleep(10);
        en[2]->stop();
    }
*/


    t[0].join();
    /*

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
    */
    
}

int main(int argc, const char * argv[])
{
    // insert code here...
    start();
    return 0;
}
