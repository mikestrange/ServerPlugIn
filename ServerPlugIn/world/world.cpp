//
//  world.cpp
//  ServerPlugIn
//  
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world.h"


//世界服务器
WorldServer* WorldServer::_instance = NULL;

WorldServer* WorldServer::getInstance()
{
    if(!_instance)
    {
        _instance = new WorldServer;
    }
    return _instance;
}

WorldServer::WorldServer()
:port(0)
{
    session = new WorldSession();
}

WorldServer::~WorldServer()
{
    SAFE_DELETE(session);
}

void WorldServer::Launch()
{
    trace("WorldServer Launch nothing");
}

void WorldServer::Launch(int port)
{
    this->port = port;
    Thread::launch(&WorldServer::ThreadServer, "NetServer");
}

void WorldServer::ThreadServer(Thread* thread)
{
    NetServer* server = WorldServer::getInstance();
    int port = WorldServer::getInstance()->port;
    if(server->Open(port))
    {
         server->PollAttemper();
    }
    SAFE_DELETE(thread);
}


void WorldServer::UnLaunch()
{
    
}

void WorldServer::HandlePacket(Client* node)
{
    session->HandlePacket(node);
}

void WorldServer::OnMainClosed(int fd, SockNode* conf)
{
    ConnServer::OnMainClosed(fd, conf);
    PlayerManager::getInstance()->RemovePlayerByFd(fd);
    PotHook::getInstance()->DelByFd(fd);
}

WorldSession* WorldServer::getSession()const
{
    return session;
}

