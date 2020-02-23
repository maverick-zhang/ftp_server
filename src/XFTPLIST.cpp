//
// Created by maverick on 2020/2/22.
//

#include "XFTPLIST.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string>
#include <iostream>

using namespace std;

void XFTPLIST::parse(const std::string &type, const std::string &msg) {

    cout << "XFTPLIST::parse " << type << "  " << msg << endl;
    if (type == "PWD")
    {
        string response_msg = "257 \"";
        response_msg += "/" + cmd_status_record->cur_worKing_dir;
        response_msg += "\" is current dir\r\n";
        cout << "PWD:  " << response_msg << endl;
        respond_cmd(response_msg);
    }
    else if(type == "LIST")
    {
        //1 链接数据通道 2 150 3 数据通道发送目录数据 4 发送完成 226 5关闭连接
        //linux目录格式
        //-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
        //连接数据通道
        data_channel_connect();
        respond_cmd("150 Here comes the directory list\r\n");
        string dir = get_dir_list(cmd_status_record->root_dir
                +cmd_status_record->cur_worKing_dir);
        cout << dir << endl;
        send_data(dir);
    }
    else if (type == "CWD")
    {
        int  pos = msg.rfind(' ') + 1;
        string path;

        path = msg.substr(pos, msg.size()-pos-2);
        cmd_status_record->cur_worKing_dir += path + "/";

        //切换目录，首先取出发送来的目录 CWD path\r\n
        respond_cmd("250 directory success changed.\r\n");
    }
    else if (type == "CDUP")
    {
        string path = cmd_status_record->cur_worKing_dir;

        if (path != "/")
        {
            int pos = path.rfind('/', path.size()-2);
            path = path.substr(0, pos+1);
            cout << "cdup path: " << path << endl;
            cmd_status_record->cur_worKing_dir = path;
        }
        respond_cmd("250 directory success changed.\r\n");


    }

}

void XFTPLIST::write(bufferevent *bev) {
    //数据发送完成
    respond_cmd("226 Transfer completed\r\n");
    //关闭连接
    if (data_bev)
    {
        bufferevent_free(data_bev);
        data_bev = nullptr;
    }

}

void XFTPLIST::event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT))
    {
        cout << "链接释放 in event_cb" << endl;
        if (data_bev)
        {
            bufferevent_free(data_bev);
            data_bev = nullptr;
        }
    }
    else if (what & BEV_EVENT_CONNECTED)
        cout << "data channel connected" << endl;
}

std::string XFTPLIST::get_dir_list(const std::string &path) {
    string dirs;
    string sh_cmd = "ls -l ";
    sh_cmd += path;
    cout << "popen:" << sh_cmd << endl;
    FILE *f = popen(sh_cmd.c_str(), "r");
    if (!f)
        return dirs;
    char buffer[1024]  {};
    for (;;)
    {
        int len = fread(buffer, 1, sizeof(buffer) - 1, f);
        if (len <= 0)break;
        buffer[len] = '\0';
        dirs += buffer;
    }
    pclose(f);
    return dirs;
}
