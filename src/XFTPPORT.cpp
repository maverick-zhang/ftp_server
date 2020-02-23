//
// Created by maverick on 2020/2/22.
//

#include <iostream>
#include <vector>
#include <string>
#include "XFTPPORT.h"

using namespace std;
void XFTPPORT::parse(const std::string &type, const std::string &msg) {
    cout << "XFTPPORT::parse " << type << "  " << msg << endl;
//    PORT n1,n2,n3,n4,n5,n6\r\n(前四个数字组成IP点分十进制，后两个为端口号)
//    PORT = n5*256 + n6
    vector<string> val;
    string tmp;
    for (int i = 5; i < msg.size(); ++i) {
        if (msg[i] == ',' or msg[i] == '\r')
        {
            val.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += msg[i];
    }
    if (val.size() != 6)
    {
        respond_cmd("501 invalid syntax\r\n");
        return;
    }
    ip = val[0] + "." + val[1] + "." + val[2] + "." + val[3];
    port =atoi(val[4].c_str()) * 256 + atoi(val[5].c_str());
    cout << "ip: " << ip << "  port:" << port << endl;
    respond_cmd("200 PORT command successful\r\n");
}
