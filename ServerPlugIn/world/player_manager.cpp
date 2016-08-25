//
//  player_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "player_manager.h"


PlayerManager* PlayerManager::_instance = NULL;

PlayerManager* PlayerManager::getInstance()
{
    if(!_instance)
    {
        _instance = new PlayerManager;
    }
    return _instance;
}


PlayerManager::PlayerManager()
{
    
}

PlayerManager::~PlayerManager()
{
    pTab.clear(block(Player* player)
    {
        SAFE_DELETE(player);
    });
}


int PlayerManager::AddPlayer(Player* player)
{
    //tid识别
    if(HasPlayer(player->token_id))
    {
        return -1;
    }
    pTab.put(player->token_id, player);
    Log::debug("用户登录 OK %d", player->user_id);
    return 0;
}

bool PlayerManager::HasPlayer(TOKEN_T user_id)
{
    return pTab.has(user_id);
}


bool PlayerManager::HasPlayerByUID(USER_T uid)
{
    HashMap<TOKEN_T, Player*>::Iterator iter;
    for(iter = pTab.begin();iter!=pTab.end();++iter)
    {
        auto player = iter->second;
        if(player->user_id == uid)
        {
            return true;
        }
    }
    return false;
}

void PlayerManager::RemovePlayer(TOKEN_T tid)
{
    auto player = pTab.remove(tid);
    if(player)
    {
        Log::debug("用户注销 OK %d", player->user_id);
    }
    SAFE_DELETE(player);
}

void PlayerManager::RemovePlayerByFd(SOCKET_T fd)
{
    HashMap<TOKEN_T, Player*>::Iterator iter;
    for(iter = pTab.begin();iter!=pTab.end();)
    {
        HashMap<TOKEN_T, Player*>::Iterator miter = iter;
        iter++;
        auto player = miter->second;
        if(player->sockfd == fd)
        {
            pTab.remove(miter);
            Log::debug("用户注销 OK %d", player->user_id);
            SAFE_DELETE(player);
        }
    }
}

Player* PlayerManager::getPlayer(TOKEN_T tid)
{
    return pTab.find(tid);
}