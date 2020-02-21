#include <iostream>
#include "XThreadPool.h"
#include <event2/listener.h>
#include <sys/socket.h>
#include "XFTPServerCMD.h"


void listen_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);

XThreadPool* XThreadPool::thread_pool = nullptr;
int main() {

    //初始化线程池
    auto thread_pool = XThreadPool::get_thread_pool();

    //创建监听socket_addr
    sockaddr_in listen {};
    listen.sin_family = AF_INET;
    listen.sin_port = htons(21);
    listen.sin_addr.s_addr = htonl(INADDR_ANY);


    //创建base和listener
    event_base* base = event_base_new();
    if (!base)
    {
        std::cerr << "创建event_base失败" << std::endl;
        return -1;
    }
    evconnlistener* listener = evconnlistener_new_bind(base, listen_cb, base, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE
                               , 16, (sockaddr*)&listen, sizeof(listen));

    if (!listener)
    {
        std::cerr << "创建监听失败" << std::endl;
        return -1;
    }

    //开始事件循环
    event_base_dispatch(base);
    evconnlistener_free(listener);
    event_base_free(base);
}
void listen_cb(struct evconnlistener *listener, evutil_socket_t sock, struct sockaddr * in_addr, int socklen, void *ctx){
    //接收到链接事件
    //创建任务,并使用线程池把任务进行分发
    XTask *task = new XFTPServerCMD();
    task->sock_fd = sock;
    XThreadPool::get_thread_pool()->dispatch(task);
}