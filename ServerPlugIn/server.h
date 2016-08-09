//
//  server.hpp
//  ServerPlugIn
//  服务器所有已知的方法
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef server_h
#define server_h

#include <vector>

#include "fd_list.h"

//最大处理
#define DEF_LISTEN_COUNT 5

class NetServer : public FdList
{
private:
    int port;
    int serId;
    struct sockaddr_in server_address;
private://sets
    fd_set readfds;
private:
    Locked del_lock;
    std::vector<int> dels;
public:
    NetServer();
    //打开服务器端口
    virtual bool open(int port,int maxfds = 5);
    //轮询端口
    virtual void poll(SCOKET_CALL perform);
    //端口是否开放
    virtual bool isRunning()const;
    //关闭端口
    virtual void closed();
    //推送关闭
    virtual void PushClose(int fd);
    
private:
    void clean_closeds(SCOKET_CALL perform);
};


#endif /* server_h */
