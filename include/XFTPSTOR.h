//
// Created by maverick on 2020/2/23.
//

#ifndef FTP_SERVER_XFTPSTOR_H
#define FTP_SERVER_XFTPSTOR_H

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>
#include "XFTPTask.h"


//文件上传
class XFTPSTOR: public XFTPTask {
public:
    void parse(const std::string & type, const std::string & msg) override;
    virtual void read(bufferevent* bev);
    virtual void event(bufferevent* bev, short what);

private:
    FILE *file = nullptr;
    char buf[1024] {};

};


#endif //FTP_SERVER_XFTPSTOR_H
