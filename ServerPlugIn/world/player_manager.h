//
//  player_manager.hpp
//  ServerPlugIn
//
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
    
private:
    HashMap<TOKEN_T, Player*> pTab;
    
public:
    PlayerManager();
    
    virtual ~PlayerManager();
    
    int AddPlayer(Player* player);
    
    void RemovePlayer(TOKEN_T tid);
    
    void RemovePlayerByFd(SOCKET_T fd);
    
    bool HasPlayer(TOKEN_T tid);
    
    Player* getPlayer(TOKEN_T tid);
    
    //uid检查
    bool HasPlayerByUID(USER_T uid);
    
private:
    static PlayerManager* _instance;
public:
    static PlayerManager* getInstance();
};



#endif /* player_manager_hpp */
