//
// Created by maverick on 2020/2/22.
//

#include <iostream>
#include "XFTPUSER.h"
using namespace std;


void XFTPUSER::parse(const std::string &type, const std::string &msg) {
    cout << "XFTPUSER::parse  " << type << "  " << msg;
    respond_cmd("230 Login success\r\n");
}
