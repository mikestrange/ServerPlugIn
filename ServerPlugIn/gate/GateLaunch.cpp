//
//  GateLaunch.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "GateLaunch.h"

STATIC_CLASS_INIT(GateLaunch);

GateLaunch::GateLaunch()
{
    proxy = new GateProxy();
}

GateLaunch::~GateLaunch()
{
    SAFE_DELETE(proxy);
}

void GateLaunch::OnConnect()
{
    
};

void GateLaunch::OnClose()
{
    
};

void GateLaunch::OnProcessPacket(SOCKET_T sockfd, PacketBuffer& packet)
{
    //房间消息就分配到房间
    Log::debug("Game Server cmd:%d type:%d",packet.getCmd(), packet.getType());
    switch(packet.getType())
    {
        case HANDLE_CLIENT_MESSAGE:
                //派送给客户端的消息
            break;
        default:
                proxy->HandlePacket(packet);
            break;
    };
};
