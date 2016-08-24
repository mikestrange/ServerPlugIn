//
//  server.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef server_hpp
#define server_hpp

#include "net.h"
#include "fd_list.h"
#include "main_loop.h"

//最大listen处理
#define DEF_LISTEN_COUNT 5

class NetServer
{
private:
    int serverId;
    FdList fd_list;
    struct sockaddr_in server_addr;
public:
    NetServer();
    
    virtual ~NetServer();
    
    virtual bool Open(int port);
    
    virtual bool isOpen()const;
    
    virtual int PollAttemper();
    
    virtual void Shut();
    
    virtual void toString();
    
private:
    SockNode* accept_handle(fd_set* fdset);
    
    void read_handle(int index, SockNode* conn, fd_set* fdset);
    
protected:
    virtual void OnRespond(SockNode* conf, char* bytes, size_t size);
    
    virtual void OnAccept(SockNode* conf);
    
    virtual void OnCloesd(SockNode* conf);
};


#endif /* server_hpp */
