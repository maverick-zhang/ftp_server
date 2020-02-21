//
// Created by maverick on 2020/2/21.
//

#ifndef FTP_SERVER_XTHREAD_H
#define FTP_SERVER_XTHREAD_H

#include <event2/event.h>
#include "XTask.h"
#include <list>
#include <mutex>


class XThread {
private:
    int t_id = 0;
    int notify_send_fd = 0;
    event_base* base = nullptr;

    //任务链表,需要线程安全,因此需要一个互斥锁
    std::mutex task_lock;
    std::list<XTask*> task_list;

public:
    //线程初始化
    void thread_init();
    //线程入口函数
    void start();
    //线程激活
    void activate();
    //添加线程处理任务
    void add_task(XTask* task);
    int get_id();
    void set_id(int i);
};


#endif //FTP_SERVER_XTHREAD_H
