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
static void activate_cb(evutil_socket_t sock_fd, short what, void * ctx)
{
    auto x_thread = (XThread*)ctx;
    if (!x_thread)
    {
        cerr << "空线程：in activate_cb func" << endl;
    }
    cout << "activate cb in thread" << this_thread::get_id() << endl;
    x_thread->activate_cb_in_thread(sock_fd, what);
}


void XThread::thread_init() {
    cout << "线程id" << this_thread::get_id() << endl;

    //创建通信管道
    int fds[2];  //0代表接收(在子线程)，1代表发送(在主线程)
    if (pipe(fds))
    {
        cerr << "pipe create failed" << endl;
        return;
    }
    notify_send_fd = fds[1];
    //创建event_base上下文
    event_config *event_conf = event_config_new();
    //base只在当前线程使用，因此设置为无锁
    event_config_set_flag(event_conf, EVENT_BASE_FLAG_NOLOCK);
    this->base =  event_base_new_with_config(event_conf);
    event_config_free(event_conf);
    if(!this->base)
    {
        //可以改为写日志
        cerr << "base created failed in thread: " << t_id << endl;
        return;
    }

    //在子线程的event_loop中添加对fd[0]的监听
    event *ev = event_new(base, fds[0], EV_READ|EV_PERSIST, activate_cb, this);
    event_add(ev, 0);

    thread th(&XThread::start, this);

    th.detach();

}

void XThread::start() {
    cout << "线程 " << t_id << " 开始" << endl;
    event_base_dispatch(this->base);


    event_base_free(this->base);

}

int XThread::get_id() {
    return t_id;
}

void XThread::set_id(int i) {
    t_id = i;
}

void XThread::activate() {

    //主线程向子线程线程发送激活消息 s:start task
    int re = write(notify_send_fd, "s", 1);
    if (re <= 0)
    {
        cerr << "线程 " << t_id <<" 激活失败" << endl;

    }

}

void XThread::add_task(XTask *task) {
    if (!task)
        return;
    task->t_id = this->t_id;
    task->base = this->base;
    task_lock.lock();
    task_list.push_back(task);
    task_lock.unlock();
}

void XThread::activate_cb_in_thread(evutil_socket_t sock_fd, short what) {


    //水平触发
    cout << "activate_cb_in_thread" << this_thread::get_id() << endl;
    char buf[2];
    int rec_len = read(sock_fd, buf, 1);
    if(rec_len <= 0)
    {
        cerr << "线程错误激活" << endl;
        return;
    }
    XTask *task = nullptr;
    //获取task并执行任务
    this->task_lock.lock();
    if(task_list.empty())
    {
        this->task_lock.unlock();
        return;
    }
    task = task_list.front();
    task_list.pop_front();
    this->task_lock.unlock();
    task->init_task();
}




