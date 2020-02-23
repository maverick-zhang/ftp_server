//
// Created by maverick on 2020/2/21.
//

#include <iostream>
#include <thread>
#include <event2/event.h>
#include "global_var.h"
#include "utils.h"
#include <event2/bufferevent.h>
#include "XFTPTask.h"
#include "XFTPServerCMD.h"


using namespace std;

XFTPTask::XFTPTask() {
}

XFTPTask::~XFTPTask() {
}

void XFTPTask::set_callback(bufferevent *bev) {
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, this);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

}

void XFTPTask::read_cb(bufferevent *bev, void *ctx) {
    XFTPTask *task = (XFTPTask*)ctx;
    task->read(bev);
}

void XFTPTask::write_cb(bufferevent *bev, void *ctx) {
    XFTPTask *task = (XFTPTask*)ctx;
    task->write(bev);

}

void XFTPTask::event_cb(bufferevent *bev, short what, void *ctx) {
    XFTPTask *task = (XFTPServerCMD*)ctx;
    task->event(bev, what);
}

void XFTPTask::read(bufferevent *bev) {

}

void XFTPTask::write(bufferevent *bev) {

}

void XFTPTask::event(bufferevent *bev, short what) {

}

void XFTPTask::parse(const std::string &type, const std::string &msg) {

}

bool XFTPTask::init_task() {
    return true;
}

void XFTPTask::respond_cmd(const std::string &msg) {
    if(!cmd_status_record or !(cmd_status_record->cmd_bev))return;
    cout << "respond_cmd" << endl;
    bufferevent_write(cmd_status_record->cmd_bev, msg.c_str(), msg.size());
}

void XFTPTask::data_channel_connect() {
    if (ip.empty() or (port == 0) or !base){
        cout << "connect data channel failed, ip and port is null" << endl;
        return;
    }
    if (data_bev)
    {
        cout << "free data bev in cmd LIST" << endl;
        bufferevent_free(data_bev);
        data_bev = nullptr;
    }

    cout << "new data bev in LIST" << endl;
    data_bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    sockaddr_in sin {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    evutil_inet_pton(AF_INET, ip.c_str(), &sin.sin_addr.s_addr);
    //设置回调和权限
    set_callback(data_bev);
    bufferevent_socket_connect(data_bev, (sockaddr*)&sin, sizeof(sin));
    if (data_bev)
    {
        cout << "连接数据通道成功" << endl;
    } else
    {
        resource_free();
        cout << "连接数据通道失败" << endl;
    }


    timeval max_time_wait {60, 0};
    bufferevent_set_timeouts(data_bev, nullptr, &max_time_wait);

}

void XFTPTask::send_data(const std::string &dir_data) {
    send_data(dir_data.c_str(), dir_data.size());
}

void XFTPTask::send_data(const char *data_Ptr, int data_size) {
    if (!data_bev)
    {
        cout << "no data channel" << endl;
        return;
    }
    cout << "目录数据已发送" << endl;
    bufferevent_write(data_bev, data_Ptr, data_size);
}

void XFTPTask::resource_free() {
    if (cmd_bev)
    {
        bufferevent_free(cmd_bev);
        cmd_bev = nullptr;
    }
    if (data_bev)
    {
        bufferevent_free(data_bev);
        data_bev = nullptr;
    }
}

