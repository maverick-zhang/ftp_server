//
// Created by maverick on 2020/2/21.
//

#include <iostream>
#include <thread>
#include <XThread.h>
#include <unistd.h>
#include <event2/event.h>
#include <event2/util.h>

using namespace std;
static void notify_cb(evutil_socket_t sock, short what, void * ctx)
{
    auto t = (XThread*)ctx;

    //水平触发
    char buf[2];
    int rec_len = read(sock, buf, 1);
    if(rec_len <= 0)
    {
        return;
    }
//    cout << "线程id: " << t->get_id() << endl;
    XTask *task = nullptr;
    //获取task并执行任务
}
void XThread::thread_init() {
    thread th(&XThread::start, this);

    th.detach();

}

void XThread::start() {
    cout << "线程id" << this_thread::get_id() << endl;

    //创建通信管道
    int fds[2];  //0代表接收，1代表发送
    if (pipe(fds))
    {
        cerr << "pipe create failed" << endl;
        return;
    }
    notify_send_fd = fds[1];
    //创建event_base上下文
    event_config *event_conf = event_config_new();
    //base只在当前线程使用，因此无锁
    event_config_set_flag(event_conf, EVENT_BASE_FLAG_NOLOCK);
    this->base =  event_base_new_with_config(event_conf);
    if(!this->base)
    {
        //可以改为写日志
        cerr << "base created failed" << endl;
        return;
    }

    //添加管道监听事件
    event *ev = event_new(base, fds[0], EV_READ|EV_PERSIST, notify_cb, this);
    event_add(ev, 0);
    event_base_dispatch(this->base);

}

int XThread::get_id() {
    return t_id;
}

void XThread::set_id(int i) {
    t_id = i;
}

void XThread::activate() {


}

void XThread::add_task(XTask *task) {
    if (!task)
        return;
    task->base = this->base;
    task_lock.lock();

    task_lock.unlock();

}



