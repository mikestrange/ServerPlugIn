//
//  server.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "server.h"

NetServer::NetServer()
:serverId(0)
{

}

NetServer::~NetServer()
{
    Shut();
}


bool NetServer::Open(int port)
{
    if(isOpen())
    {
        return false;
    }
    serverId = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    //绑定端口
    if(::bind(serverId, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0 &&
       listen(serverId, DEF_LISTEN_COUNT) == 0)
    {
        Log::warn("open server ok port = %d serid = %d", port, serverId);
        return true;
    }else{
        Log::warn("open server error port = %d", port);
    }
    serverId = 0;
    return false;
}

int NetServer::PollAttemper()
{
    if(!isOpen())
    {
        return -1;
    }
    //set
    fd_set readfds;
    //清理
    FD_ZERO(&readfds);
    //添加主体
    auto conf = fd_list.NewAccept(serverId, &readfds, server_addr);
    OnAccept(conf);
    //3秒超时
    struct timeval timeout;
    //select
    while(1)
    {
        FD_ZERO(&readfds);
        //超时时间
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        //最大的套接字(重新设置)
        int max_fd = fd_list.ResetConns(&readfds);
        if(max_fd < serverId) max_fd = serverId;
        //监听
        trace("select wait...");
        int result = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
        //端口错误
        if(result < 0)
        {
            if (errno == EINTR || errno == EAGAIN) continue;
            Log::warn("--select error:%d %d--", result, errno);
            break;
        }else{
            if(result == 0){
                trace("--select timeout--");
            }
            //所有监听的套接字
            for(int i = 0; i < fd_list.maxfds(); i++)
            {
                auto conn = fd_list.getConnInfo(i);
                if(conn == NULL) continue;
                if(conn->isClose())
                {
                    fd_list.CloseConn(i, &readfds);
                    OnCloesd(conn);
                    trace("server close to fd: %d",fd);
                }else{
                    if(FD_ISSET(conn->getFd(), &readfds))
                    {
                        if(conn->getFd() == serverId){
                            accept_handle(&readfds);
                        }else{
                            read_handle(i, conn, &readfds);
                        }
                    }
                }
            }
            //end
        }
    };
    //最后也通知
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        auto node = fd_list.CloseConn(i, &readfds);
        if(node) OnCloesd(node);
    }
    //自己关闭
    serverId = 0;
    //
    return -1;
}

SockNode* NetServer::accept_handle(fd_set* fdset)
{
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    int client_fd = accept(serverId, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
    SockNode* conn = NULL;
    if(client_fd > 0 && (conn = fd_list.NewAccept(client_fd, fdset, client_addr)))
    {
        trace("accept client: %d", client_fd);
        OnAccept(conn);
    }else{
        trace("--accept error : %d--", client_fd);
        NET_CLOSE(client_fd);
    }
    return conn;
}

void NetServer::read_handle(int index, SockNode* conn, fd_set* fdset)
{
    //缓冲
    char bytes[MAX_BUFFER];
    int ret = NET_RECV(conn->getFd(), bytes, MAX_BUFFER);
    if(ret > 0){
        trace("read fd = %d len = %d", fd, ret);
        OnRespond(conn, bytes, ret);
        memset(bytes, 0, ret);
    }else{
        if (errno == EINTR || errno == EAGAIN) return;
        trace("error or client close: %d",fd);
        fd_list.CloseConn(index, fdset);
        OnCloesd(conn);
    }
}

void NetServer::OnRespond(SockNode* conf, char *bytes, size_t size)
{
    
}

void NetServer::OnAccept(SockNode *conf)
{
    
}

void NetServer::OnCloesd(SockNode *conf)
{
    
}

bool NetServer::isOpen()const
{
    return serverId > 0;
}

void NetServer::Shut()
{
    if(isOpen())
    {
        int fd = serverId;
        serverId = 0;
        NET_CLOSE(fd);
    }
}

void NetServer::toString()
{
    fd_list.toString();
}
