//
// Created by maverick on 2020/2/22.
//

#ifndef FTP_SERVER_XFTPLIST_H
#define FTP_SERVER_XFTPLIST_H

#include "XFTPTask.h"
#include <string>


class XFTPLIST: public XFTPTask {
public:
    virtual void parse(const std::string &type, const std::string &msg);
    virtual void write(bufferevent* bev);
    virtual void event(bufferevent* bev, short what);

private:
    std::string get_dir_list(const std::string & path);
};


#endif //FTP_SERVER_XFTPLIST_H
