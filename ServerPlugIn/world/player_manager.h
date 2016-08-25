//
//  player_manager.hpp
//  ServerPlugIn
//  服务器只要知道tokenid就可以通信
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef player_manager_hpp
#define player_manager_hpp

#include "global.h"
#include "hash_map.h"
#include "player.h"

class PlayerManager
{
    STATIC_CLASS(PlayerManager);
private:
    HashMap<TOKEN_T, Player*> pTab;
    
public:
    PlayerManager();
    
    virtual ~PlayerManager();
    
    int AddPlayer(Player* player);
    
    void RemovePlayer(TOKEN_T tokenid);
    
    void RemoveSockFd(SOCKET_T sockfd);
    
    bool HasPlayer(TOKEN_T tokenid);
    
    Player* GetPlayer(TOKEN_T tokenid);
    
    //uid检查(消耗大)
    bool HasUID(USER_T userid);
};



#endif /* player_manager_hpp */
