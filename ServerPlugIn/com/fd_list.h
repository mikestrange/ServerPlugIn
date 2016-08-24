//
//  fd_list.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef fd_list_hpp
#define fd_list_hpp

#include "net.h"
#include "sock_node.h"

//服务器最大连接数目
#define MAX_CONNECTS 1024

class FdList
{
private:
    SockNode* fd_list[MAX_CONNECTS];
public:
    FdList();
private:
    void INIT_FDS();
public:
    virtual int maxfds();
    //新建
    virtual SockNode* NewAccept(int fd, fd_set* fdset, struct sockaddr_in& client_address);
    //关闭
    virtual SockNode* CloseConn(int p, fd_set* fdset);
    //重置
    virtual int ResetConns(fd_set* fdset);
    //获取
    virtual SockNode* getConnInfo(int index);
    
    virtual SockNode* SetNull(int index);
    //清理
    virtual void Clean();
    //查看
    virtual void toString();
};

#endif /* fd_list_hpp */
