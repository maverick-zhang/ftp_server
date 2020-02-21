//
// Created by maverick on 2020/2/21.
//
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include "XFTPServerCMD.h"

using namespace std;

static void read_cb(bufferevent*bev, void * arg);
static void event_cb(bufferevent*bev, short what, void * arg);
//初始化任务，运行在子线程中
bool XFTPServerCMD::init_task() {
    //监听socket bufferevent
    bufferevent *bev = bufferevent_socket_new(base, sock_fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, 0, event_cb, this);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

    timeval max_time_wait_for_read {10, 0};
    bufferevent_set_timeouts(bev, &max_time_wait_for_read, 0);

    return false;
}
static void read_cb(bufferevent*bev, void * arg)
{
    char data[1024] {};
    int len = 0;
    do{
        len = bufferevent_read(bev, data, sizeof(data));
        cout << data << endl;
    }while (len);
}
static void event_cb(bufferevent*bev, short what, void * arg){
    XFTPServerCMD *cmd = (XFTPServerCMD*)arg;

    //如果对方网络断掉或机器死机，此时可能收不到BEV_EVENT_EOF,因此需要添加超时
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT))
    {
        bufferevent_free(bev);
        delete cmd;
    }
}
