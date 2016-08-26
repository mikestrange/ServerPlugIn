//
//  logic_manager.h
//  ServerPlugIn
//  一个房间一个场景
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef home_manager_h
#define home_manager_h

#include "global.h"
#include "packet_buffer.h"
#include "hash_map.h"
#include "game_base.h"

#include "landlord/landlord_logic.h"

class HomeManager
{
    STATIC_CLASS(HomeManager);
private:
    HashMap<int32, GameBase*> gTab;
public:
    HomeManager();
    virtual ~HomeManager();
    //添加一个游戏逻辑
    bool AddLogic(int32 homeid, GameBase* target);
    //移除一个游戏逻辑
    bool RemoveLogic(int32 homeid);
    //通知(分发)
    void SendToLogic(int32 homeid, PacketBuffer& packet);
    //清理
    void CleanLogics();
};


#endif /* home_manager_h */
