//
//  net.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/9.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "net.h"


static std::map<int, int> serMap;
static Locked serLock;

static void ADD_PORT(int port, int serId)
{
    AUTO_LOCK(&serLock);
    serMap.insert(std::pair<int, int>(port, serId));
}

static bool CLOSE_PORT(int port)
{
    AUTO_LOCK(&serLock);
    std::map<int,int>::iterator iter = serMap.find(port);
    if(iter != serMap.end())
    {
        serMap.erase(iter);
        UIZ::CLOSE(iter->second);
        return true;
    }
    return false;
}

namespace UIZ
{
    bool CLOSE(int fd)
    {
        if(fd > 0)
        {
            int ret = close(fd);
            trace("close fd = %d ret = %d", fd, ret);
            return ret == 0;
        }
        trace("close fd < 0(%d)", fd);
        return false;
    }
    
    int RECV(int fd, void* bytes, size_t len)
    {
        return (int)::recv(fd, bytes, len, 0);
    }
    
    bool SEND(int fd, const void* bytes, size_t len)
    {
        if(fd > 0){
            size_t ret = send(fd, bytes, len, 0);
            trace("send to fd = %d ret = %ld, len = %ld", fd, ret, len);
            return (int)ret >= 0;
        }else{
            trace("send error: (fd <= 0) fd = %d",fd);
        }
        return false;
    }
}


namespace UIZ
{
    int RUN_SERVER(int port, FdList& fd_list, SCOKET_CALL perform)
    {
        //connect server
        int serverId = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(port);
        //绑定端口
        if(::bind(serverId, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0 &&
           listen(serverId, DEF_LISTEN_COUNT) == 0)
        {
            trace("open server ok port = %d serid = %d", port, serverId);
        }else{
            trace("open server error port = %d", port);
            return -1;
        }
        ADD_PORT(port, serverId);
        //list
        //FdList fd_list;
        fd_list.CLEAN_FDS();
        //set
        fd_set readfds;
        //客户端
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        //清理
        FD_ZERO(&readfds);
        //添加主体
        fd_list.NEW_FD(serverId, &readfds, server_addr);
        //缓冲区
        char bytes[MAX_BUFFER];
        //5秒超时
        struct timeval timeout;
        //select
        for(;;)
        {
            FD_ZERO(&readfds);
            //超时时间
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;
            //最大的套接字(重新设置)
            int max_fd = fd_list.RESET_FDS(&readfds);
            if(max_fd < serverId) max_fd = serverId;
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
                for(int i = 0; i < fd_list.maxfds(); i++)
                {
                    int fd = fd_list.ISON_FD(i, &readfds);
                    if(fd > 0)
                    {
                        if(fd == serverId)
                        {
                            int client_fd = accept(serverId, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
                            if(client_fd > 0 && fd_list.NEW_FD(client_fd, &readfds, client_addr))
                            {
                                trace("accept client: %d", client_fd);
                                perform(SOCKET_ACCEPT, client_fd, NULL, NULL);
                            }else{
                                trace("--accept error : %d--", client_fd);
                                UIZ::CLOSE(client_fd);
                            }
                        }else{
                            int ret = UIZ::RECV(fd, bytes, MAX_BUFFER);
                            if(ret > 0){
                                trace("read fd = %d len = %d", fd, ret);
                                perform(SOCKET_READ, fd, bytes, ret);
                                memset(bytes, 0, ret);
                            }else{
                                if (errno == EINTR || errno == EAGAIN) continue;
                                trace("错误和客户端de关闭 %d",fd);
                                fd_list.REMOVE_FD(i, &readfds);
                                perform(SOCKET_CLOSED, fd, NULL, NULL);
                            }
                        }
                    }else{
                        fd_list.REMOVE_FD(i, &readfds);
                    }
                }
            }
        };
        //---
        fd_list.CLEAN_FDS();
        //关闭
        STOP_SERVER(port);
        //通知
        perform(SOCKET_SELF_CLOSED, serverId, NULL, NULL);
        return -3;
    }
    
    int STOP_SERVER(int port)
    {
        if(CLOSE_PORT(port))
        {
            return 0;
        };
        return -1;
    }
}


namespace UIZ {
    int SOCKET_CONNECT(const char* host, int port)
    {
        int sockfd = 0;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            trace("socket af_inet error: %s:%d", host, port);
            return -1;
        }
        struct sockaddr_in sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family        = AF_INET;
        sock_addr.sin_addr.s_addr   = inet_addr(host);
        sock_addr.sin_port          = htons(port);
        //
        if (::connect(sockfd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr)) == -1)
        {
            trace("socket connect error: %s:%d %d", host, port, sockfd);
            return -1;
        }else{
            trace("socket connect ok: %s:%d %d", host, port, sockfd);
            return sockfd;
        }
        return -1;
    }
    
    int POLL_SOCKET(int sockfd, SCOKET_CALL perform)
    {
        //
        char bytes[MAX_BUFFER];
        for(;;)
        {
            int ret = UIZ::RECV(sockfd, bytes, MAX_BUFFER);
            if (ret > 0)
            {
                trace("socket read = %d", ret);
                perform(SOCKET_READ, sockfd, bytes, ret);
                memset(bytes, 0, ret);
            }else if(ret == 0){
                trace("##socket is close");
                break;
            }else{
                if (errno == EINTR || errno == EAGAIN){
                    
                }else{
                    trace("##socket read error: %d",ret);
                    break;
                }
            }
        }
        UIZ::CLOSE(sockfd);
        perform(SOCKET_SELF_CLOSED, sockfd, NULL, NULL);
        return -1;
    }
}