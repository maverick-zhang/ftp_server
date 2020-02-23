//
// Created by maverick on 2020/2/22.
//

#ifndef FTP_SERVER_XFTPRETR_H
#define FTP_SERVER_XFTPRETR_H

#include "XFTPTask.h"
#include <string>

class XFTPRETR: public XFTPTask {
public:
    void parse(const std::string & type, const std::string & msg) override;
    virtual void write(bufferevent* bev);
    virtual void event(bufferevent* bev, short what);
    virtual void resource_free();
private:
    FILE *file = nullptr;
    char buf[1024] {};
};


#endif //FTP_SERVER_XFTPRETR_H
