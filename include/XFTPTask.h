//
// Created by maverick on 2020/2/21.
//

#ifndef FTP_SERVER_XFTPTASK_H
#define FTP_SERVER_XFTPTASK_H

#include <event2/bufferevent.h>
#include <string>
#include "global_var.h"
#include "XTask.h"



class XFTPTask: public XTask {
public:
    virtual void read(bufferevent* bev);
    virtual void write(bufferevent* bev);
    virtual void event(bufferevent* bev, short what);
    void set_callback(bufferevent* bev);
    virtual void parse(const std::string &type, const std::string &msg);

    //链接数据通道并通告数据通道发送数据
    virtual void data_channel_connect();
    virtual void send_data(const std::string& dir_data);
    virtual void send_data(const char *data_Ptr, int data_size);

    //资源清理,仅在server_cmd的析构函数中调用
    virtual void resource_free();

    virtual bool init_task();

    //使用命令通道回复cmd消息
    void respond_cmd(const std::string &msg);

    std::string cur_worKing_dir = "";
    std::string root_dir = ROOT_DIR;
    std::string ip;
    int port = 0;

    XFTPTask *cmd_status_record = nullptr;



    XFTPTask();
    virtual ~XFTPTask();
protected:
    static void read_cb(bufferevent* bev, void* ctx);
    static void write_cb(bufferevent* bev, void* ctx);
    static void event_cb(bufferevent* bev, short what, void* ctx);

    //命令通道
    bufferevent *cmd_bev = nullptr;
    //仅在LIST和RETR子类中有,数据通道
    bufferevent *data_bev = nullptr;


};


#endif //FTP_SERVER_XFTPTASK_H
