//
// Created by maverick on 2020/2/21.
//
#include <unordered_map>
#ifndef FTP_SERVER_XFTPSERVERCMD_H
#define FTP_SERVER_XFTPSERVERCMD_H

#include "XFTPTask.h"

class XFTPServerCMD: public XFTPTask {
public:
    //初始化任务
    virtual bool init_task();
    virtual void event(bufferevent* bev, short what);
    virtual void read(bufferevent* bev);
    virtual void write(bufferevent* bev);

    //注册命令处理对象, 函数调用时还未分发到线程
    void register_task(const std::string &cmd, XFTPTask *task);



    XFTPServerCMD();
    ~XFTPServerCMD();

private:
    std::unordered_map<std::string, XFTPTask*> cmd_task;
};


#endif //FTP_SERVER_XFTPSERVERCMD_H
