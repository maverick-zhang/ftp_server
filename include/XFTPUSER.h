//
// Created by maverick on 2020/2/22.
//

#ifndef FTP_SERVER_XFTPUSER_H
#define FTP_SERVER_XFTPUSER_H

#include "XFTPTask.h"


class XFTPUSER: public XFTPTask{
public:
    virtual void parse(const std::string &type, const std::string &msg);
};


#endif //FTP_SERVER_XFTPUSER_H
