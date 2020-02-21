//
// Created by maverick on 2020/2/21.
//

#ifndef FTP_SERVER_XTHREADPOOL_H
#define FTP_SERVER_XTHREADPOOL_H


#include <cstddef>
#include <vector>
#include "XThread.h"
#include "XTask.h"

class XThreadPool {
public:
    static XThreadPool* thread_pool;
    static XThreadPool* get_thread_pool();

    //采用轮询的方法分发线程
    void dispatch(XTask* task);


    ~XThreadPool();
private:
    std::vector<XThread*> threads;
    int last_used_thread = -1;

    XThreadPool();
    class ThreadPoolAutoFree{
    public:
        ~ThreadPoolAutoFree();
    };
};


#endif //FTP_SERVER_XTHREADPOOL_H
