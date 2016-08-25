//
//  base_socket.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "base_socket.h"

bool BaseSocket::connect(const char *host, int port)
{
    if(isConnect())
    {
        return false;
    }
    listen_fd = Network::connect_server(host, port);
    return listen_fd > 0 && start();
}

void BaseSocket::run()
{
    setHandler(listen_fd);
    powder::RunMain(this, &BaseSocket::OnConnect);
    char buf[MAX_BUFFER];
    while(isConnect())
    {
        size_t ret = (size_t)handle_read(buf, MAX_BUFFER);
        if(ret == -1) break;
        if(ret == 0) continue;
        on_read(buf, ret);
        memset(buf, 0, ret);
    }
    Log::warn("socket close:%d", listen_fd);
    Disconnect();
    Network::NET_CLOSE(listen_fd);
    powder::RunMain(this, &BaseSocket::OnClose);
}

void BaseSocket::on_read(void* bytes, size_t size)
{
    powder::RunMain(this, &BaseSocket::on_main_read, MemoryPool::getInstance()->alloc_copy((char*)bytes, size), size);
};

//
void BaseSocket::on_main_read(char* bytes, size_t size)
{
    if(isConnect())
    {
        LoadBytes(bytes, size);
        try{
            while(HasPacket())
            {
                ReadBegin();
                OnProcessPacket(GetSocketFd(), self());
                ReadEnd();
            }
        }catch(Error& evt){
            Disconnect();
        }
    }
    //share
    MemoryPool::getInstance()->share(bytes);
}


void BaseSocket::OnConnect()
{
    
}

void BaseSocket::OnClose()
{
    
}

void BaseSocket::OnProcessPacket(SOCKET_T sockfd, PacketBuffer& packet)
{
    
}
