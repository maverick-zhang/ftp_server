//
// Created by maverick on 2020/2/21.
//
#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include "XFTPServerCMD.h"

using namespace std;

//初始化cmd建立新的连接
bool XFTPServerCMD::init_task() {
    //建立命令通道连接
    bufferevent *bev = bufferevent_socket_new(base, sock_fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        cout << "任务初始化失败: in init_task" << endl;
        delete this;
        return false;
    }

    //调试代码
    cout << "命令通道连接成功 (线程id)" << this->t_id << "  " << this_thread::get_id() << endl;
    this->set_callback(bev);
    this->cmd_bev = bev;
    this->cmd_status_record = this;

    //设置连接超时
    timeval max_time_wait{60, 0};
    bufferevent_set_timeouts(bev, &max_time_wait, 0);
    //创建链接之后，发送消息给客户端
    string msg = "220 Welcome to Maverick's FTP Server\r\n";
    bufferevent_write(bev, msg.c_str(), msg.size());

    return true;
}

XFTPServerCMD::XFTPServerCMD() = default;

XFTPServerCMD::~XFTPServerCMD() {
    if (cmd_bev)
        bufferevent_free(cmd_bev);
    bool list_task_deleted = false;
    for(auto const & pairs: cmd_task)
    {

        if (pairs.first == "LIST" or pairs.first == "PWD" or pairs.first == "CWD" or pairs.first == "CDUP")
        {
            if(!list_task_deleted)
            {
                pairs.second->resource_free();
                delete pairs.second;
                list_task_deleted = true;
            }
        } else
        {
            pairs.second->resource_free();
            delete pairs.second;
        }
    }
}

void XFTPServerCMD::event(bufferevent *bev, short what) {
    //如果对方网络断掉或机器死机，此时可能收不到BEV_EVENT_EOF,因此需要添加超时
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT))
    {
        cout << "链接释放 in event_cb" << endl;
//        bufferevent_free(bev);   //统一放在析构函数中进行释放
        delete this;
    }
}


//收到客户端的命令，先解析出命令类别，进行任务分发
void XFTPServerCMD::read(bufferevent *bev) {
    cout << "read_cb" << endl;
    char data[1024] {};
    int len = 0;
    for(;;)
    {
        len = bufferevent_read(bev, data, sizeof(data));
        if (len <= 0)
        {
            data[0] = '\0';
            break;
        }
        //debug用代码
        cout << "收到的指令：" << data <<" id: " << t_id << endl;
        //分析命令类型，分发到处理对象
        string cmd;
        for (int i = 0; i < len; ++i) {
            if (data[i] == ' ' or data[i] == '\r')
                break;
            cmd += data[i];
        }
        cout << "命令类型为: [" << cmd << "]" << endl;
        if (cmd_task.find(cmd) != cmd_task.end())
        {
            XFTPTask *task = cmd_task[cmd];
            //向该命令类传递相关的数据
            task->cmd_status_record = this;
//            task->cmd_bev = this->cmd_bev;
            task->ip = ip;
            task->port = port;
            task->base = base;
            task->parse(cmd, data);
            //从PORT命令中解析出端口并存储在ServerCMD中
            if (cmd == "PORT")
            {
                ip = task->ip;
                port = task->port;
            }
        }
        else
        {
            string msg = "200 OK\r\n";
            bufferevent_write(bev, msg.c_str(), msg.size());
//            cerr << "命令类型错误: " << cmd << endl;
        }
    }
}

void XFTPServerCMD::write(bufferevent *bev) {
    cout << "write_cb_in_server_cmd" << endl;
}

void XFTPServerCMD::register_task(const std::string &cmd, XFTPTask *task)
{
    if (!task)
    {
        std::cerr << "register task is null" << std::endl;
        return;
    }
    if (cmd.empty())
    {
        std::cerr << "invalid command" << std::endl;
        return;
    }
    //对已经注册的cmd,直接报错
    if (cmd_task.find(cmd) != cmd_task.end())
    {
        std::cerr << cmd << " has registered" << std::endl;
        return;
    }
    cmd_task[cmd] = task;

}


