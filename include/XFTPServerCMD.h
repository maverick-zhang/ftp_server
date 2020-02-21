//
// Created by maverick on 2020/2/21.
//

#ifndef FTP_SERVER_XFTPSERVERCMD_H
#define FTP_SERVER_XFTPSERVERCMD_H

#include "XTask.h"

class XFTPServerCMD: public XTask {
public:
    //初始化任务
    virtual bool init_task();
    XFTPServerCMD();
    ~XFTPServerCMD();
};


#endif //FTP_SERVER_XFTPSERVERCMD_H
