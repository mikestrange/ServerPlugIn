//
//  GameLaunch.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "GameLaunch.h"


STATIC_CLASS_INIT(GameLaunch);

GameLaunch::GameLaunch()
{
    proxy = new GameProxy();
}

GameLaunch::~GameLaunch()
{
    SAFE_DELETE(proxy);
}

GameProxy* GameLaunch::getProxy() const
{
    return proxy;
}

void GameLaunch::OnConnect()
{
    
};

void GameLaunch::OnClose()
{
    
};

void GameLaunch::OnProcessPacket(SOCKET_T sockfd, PacketBuffer& packet)
{
    //房间消息就分配到房间
    Log::debug("Game Server cmd:%d type:%d",packet.getCmd(), packet.getType());
    switch(packet.getType())
    {
        case HANDLE_GAME_MESSAGE:
            HomeManager::getInstance()->SendToLogic(packet.getViewId(), packet);
        break;
        default:
            proxy->HandlePacket(packet);
        break;
    }
};

