//
// Created by maverick on 2020/2/23.
//

#include "XFTPSTOR.h"
#include <string>
#include <iostream>

using namespace std;

void XFTPSTOR::parse(const std::string &type, const std::string &msg) {

    int  pos = msg.rfind(' ') + 1;
    string file_path;
    file_path = msg.substr(pos, msg.size()-pos-2);

    //传递文件名
    file_path = cmd_status_record->root_dir + cmd_status_record->cur_worKing_dir + file_path;
    cout << "文件路径: " << file_path << endl;

    file = fopen(file_path.c_str(), "wb");
    if(file)
    {
        data_channel_connect();
        respond_cmd("125 file ok\r\n");
        bufferevent_trigger(data_bev, EV_READ, 0);
    } else
        respond_cmd("450 file open failed\r\n");
}


void XFTPSTOR::event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT))
    {
        cout << "链接释放 in event_cb" << endl;
        if(file)
        {
            fclose(file);
            file = nullptr;
        }
        respond_cmd("226 Transfer complete\r\n");
        bufferevent_free(bev);
        this->data_bev = nullptr;
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        //数据通道连接成功触发写入事件
        cout << "data channel connected" << endl;
    }
}

void XFTPSTOR::read(bufferevent *bev) {
    if (!file)return;
    for(;;){
        int len = bufferevent_read(bev, buf, sizeof(buf));
        if (len <= 0)break;
        fwrite(buf, 1, len, file);
    }
}
