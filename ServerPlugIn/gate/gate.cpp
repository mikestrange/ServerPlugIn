//
//  GateServer.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "gate.h"

STATIC_CLASS_INIT(GateServer);

void GateServer::OnRemove(SOCKET_T sockfd)
{
    //移除登录的玩家
    
};

void GateServer::OnRegister(SOCKET_T sockfd, SocketHandler* sock)
{
    
};

void GateServer::OnProcessPacket(SOCKET_T sockfd, SocketHandler& packet)
{
    //会话
    GateSession::getInstance()->OnPacketHandle(sockfd, packet);
};
