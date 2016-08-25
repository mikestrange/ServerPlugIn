//
//  player_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "player_manager.h"

STATIC_CLASS_INIT(PlayerManager);

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

bool PlayerManager::HasPlayer(TOKEN_T tokenid)
{
    return pTab.has(tokenid);
}


bool PlayerManager::HasUID(USER_T userid)
{
    HashMap<TOKEN_T, Player*>::Iterator iter;
    for(iter = pTab.begin();iter!=pTab.end();++iter)
    {
        auto player = iter->second;
        if(player->user_id == userid)
        {
            return true;
        }
    }
    return false;
}

void PlayerManager::RemovePlayer(TOKEN_T tokenid)
{
    auto player = pTab.remove(tokenid);
    if(player)
    {
        Log::debug("用户注销 OK %d", player->user_id);
    }
    SAFE_DELETE(player);
}

void PlayerManager::RemoveSockFd(SOCKET_T sockfd)
{
    HashMap<TOKEN_T, Player*>::Iterator iter;
    for(iter = pTab.begin();iter!=pTab.end();)
    {
        HashMap<TOKEN_T, Player*>::Iterator miter = iter;
        iter++;
        auto player = miter->second;
        if(player->sockfd == sockfd)
        {
            pTab.remove(miter);
            Log::debug("用户注销 OK %d", player->user_id);
            SAFE_DELETE(player);
        }
    }
}

Player* PlayerManager::GetPlayer(TOKEN_T tokenid)
{
    return pTab.find(tokenid);
}