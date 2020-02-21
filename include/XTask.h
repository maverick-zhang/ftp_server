//
// Created by maverick on 2020/2/21.
//

#ifndef FTP_SERVER_XTASK_H
#define FTP_SERVER_XTASK_H

//任务基类，这是一个接口类，即无对象
class XTask {
public:
    struct event_base *base = nullptr;
    int sock_fd = 0;
    virtual bool init_task() = 0;
};


#endif //FTP_SERVER_XTASK_H
