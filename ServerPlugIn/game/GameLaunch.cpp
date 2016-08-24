//
//  GameLaunch.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "GameLaunch.h"


GameLaunch* GameLaunch::_instance = NULL;

GameLaunch* GameLaunch::getInstance()
{
    if(!_instance)
    {
        _instance = new GameLaunch;
    }
    return _instance;
}


GameLaunch::GameLaunch()
:host(NULL)
,port(0)
{
    manager = new SceneManager();
    proxy = new GameProxy();
}

GameLaunch::~GameLaunch()
{
    SAFE_DELETE(manager);
    SAFE_DELETE(proxy);
}

void GameLaunch::Launch()
{
    trace("GameLaunch Launch nothing");
}

void GameLaunch::Launch(const char* host, int port)
{
    this->host = host;
    this->port = port;
    Thread::launch(&GameLaunch::ThreadSocket);
}

void GameLaunch::ThreadSocket(Thread* thread)
{
    GameLaunch* game = GameLaunch::getInstance();
    if(game->Connect(game->host, game->port))
    {
        game->PollAttemper();
    }
    SAFE_DELETE(thread);
}

void GameLaunch::UnLaunch()
{
    trace("UnLaunch nothing");
}

GameProxy* GameLaunch::getProxy() const
{
    return proxy;
}

SceneManager* GameLaunch::getManager() const
{
    return manager;
}

void GameLaunch::HandlePacket(PacketBuffer &packet)
{
    //房间消息就分配到房间
    Log::debug("Game Server cmd:%d type:%d",packet.cmd, packet.type);
    switch(packet.type)
    {
        case HANDLE_GAME_MESSAGE:
                manager->SendToLogic(packet.view_id, packet);
            break;
        default:
                proxy->HandlePacket(packet);
            break;
    }
}
