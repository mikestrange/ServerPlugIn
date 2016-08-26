//
//  logic_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "home_manager.h"

STATIC_CLASS_INIT(HomeManager);

HomeManager::HomeManager()
{
    //init
    //AddLogic(1002, new LandlordLogic(1001));
}

HomeManager::~HomeManager()
{
    CleanLogics();
}

bool HomeManager::AddLogic(int32 homeid, GameBase *target)
{
    if(gTab.has(homeid)){
        trace("add error: have viewid [%d] delete this", homeid);
        SAFE_DELETE(target);
    }else{
        gTab.put(homeid, target);
        Log::debug("启动一个房间: %d", homeid);
        target->Launch();
        return true;
    }
    return false;
}

bool HomeManager::RemoveLogic(int32 homeid)
{
    auto v = gTab.remove(homeid);
    if(v){
        Log::debug("注销一个房间: %d", homeid);
        v->UnLaunch();
        SAFE_DELETE(v);
        return true;
    }else{
        trace("remove error: no viewid %d", homeid);
    }
    return false;
}

void HomeManager::SendToLogic(int32 homeid, PacketBuffer &packet)
{
    auto v = gTab.find(homeid);
    if(v)
    {
        trace("send goto viewid ok: %d", homeid);
        v->HandlePacket(packet);
    }else{
        trace("send error: no viewid %d", homeid);
    }
}

void HomeManager::CleanLogics()
{
    Log::debug("清理所有房间");
    gTab.clear(block(GameBase* v)
    {
        v->UnLaunch();
        SAFE_DELETE(v);
    });
}

