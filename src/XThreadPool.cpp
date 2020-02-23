//
// Created by maverick on 2020/2/21.
//
#include <iostream>
#include "XThreadPool.h"
#include "XThread.h"
#include "global_var.h"

using namespace std;

XThreadPool *XThreadPool::get_thread_pool() {
    if (thread_pool)
        return thread_pool;
    thread_pool = new XThreadPool();
    static auto gc = ThreadPoolAutoFree();
    return thread_pool;
}


XThreadPool::XThreadPool() :threads(){
    for (int i = 0; i < THREADS_NUMBER ; ++i) {
        //创建线程池中的线程
        XThread *t = new XThread();
        //对线程进行初始化
        t->thread_init();
        t->set_id(i);
        threads.push_back(t);
        std::cout << "线程" << i << "创建成功" << std::endl; //debug only
    }
}

XThreadPool::~XThreadPool() {
    for (XThread *thread: threads) {
        delete thread;
        thread = nullptr;
    }
    std::cout << "线程全部销毁" << std::endl;
}

void XThreadPool::dispatch(XTask *task) {
    //获取到任务，轮询线程池找到下一个线程并把任务提交给线程
    if (!task)
    {
        cerr << "task should not be null" << endl;
        return;

    }
    int t_id = (last_used_thread+1) % THREADS_NUMBER;
    last_used_thread += 1;
    XThread *x_thread = threads[t_id];

    //添加任务激活线程
    x_thread->add_task(task);
    //激活线程后就会触发notify的回调
    x_thread->activate();


}

XThreadPool::ThreadPoolAutoFree::~ThreadPoolAutoFree() {

    delete thread_pool;
    thread_pool = nullptr;
}
XThreadPool* XThreadPool::thread_pool = nullptr;
