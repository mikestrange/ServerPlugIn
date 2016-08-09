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
#include <map>

#include "net_base.h"


//最大处理
#define DEF_LISTEN_COUNT 5

class NetServer : public NetBase
{
private:
    int port;
    int serId;
    struct sockaddr_in server_address;
private://sets
    fd_set readfds;
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
    //移除一个连接(外部调用)
    virtual void closed(int fd);
};


#endif /* server_h */
