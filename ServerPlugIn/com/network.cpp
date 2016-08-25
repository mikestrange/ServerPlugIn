//
//  network.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/24.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "network.h"


//class Network

Network::Network()
:maxfds(0)
,isrunning(false)
,listen_fd(0)
{
    
}

Network::~Network()
{
    
}

bool Network::create_listener(int port)
{
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    //绑定端口
    if(::bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0 &&
       listen(listen_fd, MAX_LISTENE) == 0)
    {
        isrunning = true;
        Log::warn("open server ok port = %d serid = %d", port, listen_fd);
        return true;
    }else{
        Log::warn("open server error port = %d", port);
    }
    listen_fd = 0;
    return false;
}


int Network::select_server()
{
    FD_ZERO(&m_rset);
    struct timeval timeout;
    //
    register_event(listen_fd);
    //reset
    char buf[MAX_BUFFER];
    //running
    while(isrunning)
    {
        SET_TIMEOUT(timeout, 1, 0);
        fd_set m_tmp_rset = m_rset;
        int ready = select(maxfds + 1, &m_tmp_rset, NULL, NULL, &timeout);
        if(ready < 0)
        {
            if (errno == EINTR || errno == EAGAIN) continue;
            Log::warn("--select error:%d %d--", ready, errno);
        }else{
            if(FD_ISSET(listen_fd, &m_tmp_rset))
            {
                handle_accept();
            }
            //轮询所有(第一个为端口本身不在列表中)
            for(int i = 1; i < MAX_SERVER; i++)
            {
                auto p_socket = sock_list[i];
                if(p_socket == NULL) continue;
                if(p_socket->isConnect()){
                    if(FD_ISSET(p_socket->GetSocketFd(), &m_tmp_rset))
                    {
                        int ret = p_socket->handle_read(buf, MAX_BUFFER);
                        //忽略0
                        if(ret > 0){
                            on_read(p_socket, buf, ret);
                            //reset
                            memset(buf, 0, ret);
                        }
                        //－1 close
                        if(ret == -1) handle_close(i);
                    }
                }else{
                    handle_close(i);
                }
            }
        }
    }
    //端口在内
    for(int i = 0; i < MAX_SERVER; i++)
    {
        handle_close(i);
    }
    return 0;
}

SocketHandler* Network::alloc_sockethandler(int index, SOCKET_T fd)
{
    return new SocketHandler(index, fd);
}

void Network::register_event(SOCKET_T fd)
{
    SocketHandler* sock = NULL;
    for(int i = 0; i < MAX_SERVER; i++)
    {
        if(sock_list[i] == NULL)
        {
            sock = alloc_sockethandler(i, fd);
            sock_list[i] = sock;
            FD_SET(fd, &m_rset);
            //最大fd
            if(fd > maxfds) maxfds = fd;
            if(maxfds < listen_fd) maxfds = listen_fd;
            break;
        }
    }
    //注册成功后通知
    if(sock)
    {
        on_register(sock);
    }else{
        Network::NET_CLOSE(fd);
    }
}

void Network::handle_accept()
{
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    SOCKET_T newfd = accept(listen_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
    if(newfd == INVALID_SOCKET) return;
    //去注册
    register_event(newfd);
}

void Network::handle_close(int index)
{
    auto p_socket = sock_list[index];
    if(p_socket)
    {
        sock_list[index] = NULL;
        //关闭处理
        p_socket->Disconnect();
        //清理套接字
        SOCKET_T fd = p_socket->GetSocketFd();
        FD_CLR(fd, &m_rset);
        Network::NET_CLOSE(fd);
        //mainLoop处理buf
        on_close(p_socket);
    }
}

void Network::on_register(SocketHandler* sock)
{
    //mainLoop处理
    Log::debug("accept and reg socket ok: %d", sock->GetSocketFd());
}

void Network::on_read(SocketHandler* sock, char* str, size_t size)
{
    //main loop
    Log::debug("read ok: %d %d", size, sock->GetSocketFd());
}

void Network::on_close(SocketHandler* sock)
{
    //main loop
    Log::debug("close socket handle:%d", sock->GetSocketFd());
}


void Network::stop_server()
{
    isrunning = false;
}

void Network::toString()
{
    
}

//static
SOCKET_T Network::connect_server(const char *ip, int port)
{
    SOCKET_T listen_fd;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Log::warn("socket af_inet error: %s:%d", ip, port);
    }else{
        struct sockaddr_in sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family        = AF_INET;
        sock_addr.sin_addr.s_addr   = inet_addr(ip);
        sock_addr.sin_port          = htons(port);
        //
        if (::connect(listen_fd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr)) == -1)
        {
            Log::warn("socket connect error: %s:%d %d", ip, port, listen_fd);
        }else{
            Log::info("socket connect ok: %s:%d %d", ip, port, listen_fd);
            return listen_fd;
        }
    }
    return -1;
}


bool Network::NET_CLOSE(SOCKET_T fd)
{
    return ::close(fd) == 0;
}

int Network::NET_RECV(SOCKET_T fd, void* bytes, size_t len)
{
    return (int)::recv(fd, bytes, len, 0);
}

int Network::NET_SEND(SOCKET_T fd, const void* bytes, size_t len)
{
    return (int)::send(fd, bytes, len, 0);
}

