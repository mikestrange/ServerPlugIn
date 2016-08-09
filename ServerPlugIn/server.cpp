//
//  server.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "server.h"

#include "time_utils.h"

NetServer::NetServer()
:port(0)
,serId(0)
{
    
}

bool NetServer::isRunning()const
{
    return serId > 0;
}

bool NetServer::open(int port, int maxfds)
{
    if(isRunning())
    {
        return false;
    }
    int serverId = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    //绑定端口
    if(::bind(serverId, (struct sockaddr *)&server_address, sizeof(server_address)) == 0 &&
       listen(serverId, DEF_LISTEN_COUNT) == 0)
    {
        FdList::maxfds(maxfds);
        serId = serverId;
        this->port = port;
        trace("open server ok port = %d serid = %d max = %d", port, serId, MAX_SERVER);
        return true;
    }
    serId = 0;
    trace("open server error port = %d", port);
    return false;
}

void NetServer::poll(SCOKET_CALL perform)
{
    //客户端
    struct sockaddr_in client_address;
    int client_len = sizeof(client_address);
    //清理
    FD_ZERO(&readfds);
    //初始化
    init_fds();
    //添加主体
    add(serId);
    //缓冲区
    char bytes[MAX_BUFFER];
    //3秒超时
    struct timeval timeout = {3, 0};
    int maxfds = FdList::maxfds();
    //轮询
    while(isRunning())
    {
        FD_ZERO(&readfds);
        //关闭添加的
        clean_closeds(perform);
        //最大的套接字(重新设置)
        int max_fd = reset(&readfds);
        if(max_fd < serId) max_fd = serId;
        //监听
        int result = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
        //端口错误
        if(result < 0)
        {
            if (errno == EINTR || errno == EAGAIN) continue;
            //
            trace("--select error:%d %d--", result, errno);
            break;
        }else if(result == 0){
            trace("--select timeout--");
        }else{
            for(int i = 0; i < maxfds; i++)
            {
                int fd = isset(i, &readfds);
                if(fd > 0)
                {
                    if(fd == serId)
                    {
                        int client_fd = accept(serId, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
                        if(client_fd > 0 && add(client_fd))
                        {
                            trace("accept client: %d", client_fd);
                            perform(SOCKET_ACCEPT, client_fd, NULL, NULL);
                        }else{
                            trace("--accept error : %d--", client_fd);
                            net::close_fd(client_fd);
                        }
                    }else{
                        int ret = net::recv_fd(fd, bytes, MAX_BUFFER);
                        if(ret > 0){
                            trace("read fd = %d len = %d", fd, ret);
                            perform(SOCKET_READ, fd, bytes, ret);
                            memset(bytes, 0, ret);
                        }else{
                            if (errno == EINTR || errno == EAGAIN) continue;
                            //trace("错误和客户端de关闭 %d",fd);
                            remove(i, &readfds);
                            perform(SOCKET_CLOSED, fd, NULL, NULL);
                        }
                    }
                }
            }
        }
    };
    //清理其他的关闭(会通知未移除的客户端)
    clean(&readfds, perform);
    //服务器套接字设置为0
    closed();
    //通知
    perform(SOCKET_SELF_CLOSED, NULL, NULL, NULL);
}

//关闭服务器
void NetServer::closed()
{
    if(isRunning())
    {
        int _fd = serId;
        serId = 0;
        net::close_fd(_fd);
    }else{
        trace("this server is stop");
    }
}

void NetServer::PushClose(int fd)
{
    if(isRunning())
    {
        AUTO_LOCK(&del_lock);
        dels.push_back(fd);
    }
}

//关闭需要被关闭的连接
void NetServer::clean_closeds(SCOKET_CALL perform)
{
    AUTO_LOCK(&del_lock);
    std::vector<int>::iterator iter;
    for(iter = dels.begin();iter != dels.end(); ++iter)
    {
        shut(*iter, &readfds, perform);
    }
    dels.clear();
}












