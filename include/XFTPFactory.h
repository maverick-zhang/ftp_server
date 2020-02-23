//
// Created by maverick on 2020/2/21.
//

#ifndef FTP_SERVER_XFTPFACTORY_H
#define FTP_SERVER_XFTPFACTORY_H

#include <XTask.h>


//工厂，同样是单例模式
class XFTPFactory {
public:
    static XFTPFactory* get_factory();

    XTask* create_task();

private:
    static XFTPFactory *factory;
    XFTPFactory();
    class FactoryAutoDelete{
    public:
        ~FactoryAutoDelete();
    };
};



#endif //FTP_SERVER_XFTPFACTORY_H
