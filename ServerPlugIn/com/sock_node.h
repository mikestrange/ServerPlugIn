//
//  conn_info.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef conn_info_hpp
#define conn_info_hpp

#include <netinet/in.h>

#include "global.h"
#include "lock.h"
#include "net.h"

#define FD_NULL 0

typedef enum _FD_STATUS
{
    FD_OFF = 0,
    FD_ON = 1
}FD_STATUS;

class SockNode
{
private:
    int fd;
    int isOn;
    struct sockaddr_in addr;
public:
    SockNode();
    
    SockNode(int fd, struct sockaddr_in& addr);
    
    virtual ~SockNode();
    
protected:
    void OnConnect(int fd, struct sockaddr_in& addr);
    
public:
    int setClose();
    
    int getFd();
    
    bool isClose()const;
    
    bool isConnect()const;
    
    bool isSetFd()const;
    
    void Send(const void* bytes, size_t size);
};

#endif /* conn_info_hpp */

