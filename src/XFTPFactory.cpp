//
// Created by maverick on 2020/2/21.
//

#include "XFTPFactory.h"
#include "XFTPServerCMD.h"
#include "XFTPUSER.h"
#include "XFTPLIST.h"
#include "XFTPPORT.h"
#include "XFTPRETR.h"
#include "XFTPSTOR.h"

XFTPFactory *XFTPFactory::get_factory() {
    if (factory)
        return factory;
    static FactoryAutoDelete factory_gc;
    factory = new XFTPFactory();
    return factory;
}

XTask *XFTPFactory::create_task() {
    XFTPServerCMD *cmd = new XFTPServerCMD();
    //注册ftp消息处理
    cmd->register_task("USER", new XFTPUSER());
    auto list_task = new XFTPLIST();
    cmd->register_task("PWD", list_task);
    cmd->register_task("LIST", list_task);
    cmd->register_task("CWD", list_task);
    cmd->register_task("CDUP", list_task);
    cmd->register_task("RETR", new XFTPRETR());
    cmd->register_task("STOR", new XFTPSTOR());
    cmd->register_task("PORT", new XFTPPORT());


    return cmd;
}

XFTPFactory::XFTPFactory() {


}

XFTPFactory::FactoryAutoDelete::~FactoryAutoDelete() {
    if(factory)
    {
        delete factory;
        factory = nullptr;
    }

}
XFTPFactory* XFTPFactory::factory = nullptr;
