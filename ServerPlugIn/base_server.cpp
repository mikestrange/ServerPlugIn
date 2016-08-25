//
//  base_server.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "base_server.h"

BaseServer::~BaseServer()
{
    sockMap.clear(block(SocketHandler* sock)
    {
        SAFE_DELETE(sock);
    });
}

bool BaseServer::Launch(int port)
{
    return create_listener(port) && start();
}


void BaseServer::run()
{
    select_server();
}


void BaseServer::on_read(SocketHandler* sock, char* bytes, size_t size)
{
    powder::RunMain(this, &BaseServer::on_main_read,
                    sock,
                    MemoryPool::getInstance()->alloc_copy(bytes, size),
                    size);
};

void BaseServer::on_register(SocketHandler* sock)
{
    powder::RunMain(this, &BaseServer::on_main_register, sock);
};

void BaseServer::on_close(SocketHandler* sock)
{
    powder::RunMain(this, &BaseServer::on_main_close, sock);
};

//
void BaseServer::on_main_read(SocketHandler* sock, char* bytes, size_t size)
{
    if(sock->isConnect())
    {
        sock->LoadBytes(bytes, size);
        try{
            while(sock->HasPacket())
            {
                sock->ReadBegin();
                OnProcessPacket(sock->GetSocketFd(), *sock);
                sock->ReadEnd();
            }
        }catch(Error& evt){
            sock->Disconnect();
        }
    }
    //share
    MemoryPool::getInstance()->share(bytes);
}

void BaseServer::on_main_register(SocketHandler* sock)
{
    if(sockMap.has(sock->GetSocketFd()))
    {
        sock->Disconnect(); //被占线
    }else{
        sockMap.put(sock->GetSocketFd(), sock);
        //通知
        OnRegister(sock->GetSocketFd(), sock);
    }
}

void BaseServer::on_main_close(SocketHandler* sock)
{
    if(sockMap.remove(sock->GetSocketFd()))
    {
        OnRemove(sock->GetSocketFd());
    }
    SAFE_DELETE(sock);
}

//do something
SocketHandler* BaseServer::getSocketHandler(SOCKET_T sockfd)
{
    return sockMap.find(sockfd);
}

void BaseServer::OnProcessPacket(SOCKET_T sockfd, SocketHandler& packet)
{
    
}

void BaseServer::OnRegister(SOCKET_T sockfd, SocketHandler* sock)
{
    
}

void BaseServer::OnRemove(SOCKET_T sockfd)
{
    
}