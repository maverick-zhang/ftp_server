#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <event2/listener.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "global_var.h"
#include "XFTPServerCMD.h"
#include "XThreadPool.h"
#include "XFTPFactory.h"

//监听回调函数
void listen_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
char ROOT_DIR[64];
int main(int argc, char *const * argv) {

//    if (argc!=2 )
//        return -1;
//    if (strlen(argv[1]) > 63)
//    {
//        std::cerr << "路径过长" << std::endl;
//        return -1;
//    }
//    if (access(argv[1], 6) < 0)
//    {
//        std::cerr << "目录文件不存在，或无权限" << std::endl;
//        return -1;
//    }
//    struct stat s_buf {};
//    stat(argv[1], &s_buf);
//    if (!S_ISDIR(s_buf.st_mode))
//    {
//        std::cerr << "非目录" << std::endl;
//        return -1;
//    }
//    strcpy(ROOT_DIR, argv[1]);

    strcpy(ROOT_DIR, "/home/maverick/");


    //初始化线程池
    auto thread_pool = XThreadPool::get_thread_pool();

    //创建监听socket_addr
    sockaddr_in listen {};
    listen.sin_family = AF_INET;
    listen.sin_port = htons(5001);
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
    //接收到连接事件
    //创建连接任务,并使用线程池把任务进行分发
    XTask *task = XFTPFactory::get_factory()->create_task();
    task->sock_fd = sock;
    XThreadPool::get_thread_pool()->dispatch(task);
}