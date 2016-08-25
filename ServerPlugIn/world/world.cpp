//
//  world.cpp
//  ServerPlugIn
//  
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world.h"

STATIC_CLASS_INIT(WorldServer);


void WorldServer::OnRemove(SOCKET_T sockfd)
{
    //用户
    PlayerManager::getInstance()->RemoveSockFd(sockfd);
    //钩子
    PotHook::getInstance()->DelBySockFd(sockfd);
};

void WorldServer::OnRegister(SOCKET_T sockfd, SocketHandler* sock)
{
    
};

void WorldServer::OnProcessPacket(SOCKET_T sockfd, SocketHandler& packet)
{
    //会话
    WorldSession::getInstance()->OnPacketHandle(sockfd, packet);
};


