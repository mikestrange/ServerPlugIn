//
//  net_base.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef fd_list_h
#define fd_list_h

#include "net.h"

//服务器最大连接数目
#define MAX_CONNECTS 1024

typedef struct comefd
{
    int fd;
    int isOn;//状态，不代表存在
    struct sockaddr_in addr;
}comefd;

class FdList
{
private:
    comefd fd_list[MAX_CONNECTS];
public:
    FdList();
private:
    void INIT_FDS();
public:
    virtual void CLEAN_FDS();
    
    virtual int maxfds();
    
    virtual bool NEW_FD(int fd, fd_set* fdset, struct sockaddr_in& client_address);
    
    virtual bool REMOVE_FD(int p, fd_set* fdset);
    
    virtual int RESET_FDS(fd_set* fdset);
    
    virtual int ISON_FD(int p, fd_set* fdset);
    
public:
    //外界调用(推送关闭)
    virtual void PUSH_CLOSE(int fd);
    
    virtual void toString();
};

#endif /* net_base_hpp */
