//
// Created by maverick on 2020/2/22.
//

#include "XFTPRETR.h"
#include <string>
#include <sys/stat.h>
#include <event.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void XFTPRETR::parse(const std::string &type, const std::string &msg) {
    int  pos = msg.rfind(' ') + 1;
    string file_path;
    file_path = msg.substr(pos, msg.size()-pos-2);

    //传递文件名
    file_path = cmd_status_record->root_dir + cmd_status_record->cur_worKing_dir + file_path;
    cout << "文件路径: " << file_path << endl;

    if (access(file_path.c_str(), 4) < 0)
    {
        respond_cmd("450 file open failed.\r\n");
        std::cout << "文件不存在，或无读取权限" << std::endl;
        return;
    }
    struct stat s_buf {};
    stat(file_path.c_str(), &s_buf);
    if (!S_ISREG(s_buf.st_mode))
    {
        respond_cmd("450 file open failed.\r\n");
        std::cerr << "非文件" << std::endl;
        return ;
    }
    file = fopen(file_path.c_str(), "rb");
    if (!file)
    {
        respond_cmd("450 file open failed.\r\n");
        cerr << "文件打开失败" << endl;
        return;
    }
    respond_cmd("150 file ok.\r\n");
    data_channel_connect();
    if (!data_bev)
    {
        respond_cmd("450 file open failed.\r\n");
        cout << "no data channel" << endl;
        return;
    }
    bufferevent_trigger(data_bev, EV_WRITE, 0);
}

void XFTPRETR::write(bufferevent *bev) {
    if (!file)
        return;
    int len = fread(buf, 1, sizeof(buf), file);
    if (len > 0)
        bufferevent_write(bev, buf, len);
    else
    {
        fclose(file);
        file = nullptr;
        //读到文件结尾
        respond_cmd("226 File Transfer completed\r\n");
        cout << "文件传输完成" << endl;
        bufferevent_free(bev);
    }

}

void XFTPRETR::event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT))
    {
        cout << "链接释放 in event_cb" << endl;
        bufferevent_free(bev);
        this->data_bev = nullptr;
        if(file)
            fclose(file);
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        //数据通道连接成功触发写入事件
        cout << "data channel connected" << endl;
    }
}

void XFTPRETR::resource_free() {
    XFTPTask::resource_free();
    if (file)
    {
        fclose(file);
        file = nullptr;
    }
}
