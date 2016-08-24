//
//  logic_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "scene_manager.h"

//

SceneManager::SceneManager()
{
    //init
    //AddLogic(1002, new LandlordLogic(1001));
}

SceneManager::~SceneManager()
{
    CleanLogics();
}

bool SceneManager::AddLogic(int viewid, GameBase *target)
{
    if(gTab.has(viewid)){
        trace("add error: have viewid [%d] delete this", viewid);
        SAFE_DELETE(target);
    }else{
        gTab.put(viewid, target);
        Log::debug("启动一个房间: %d", viewid);
        target->Launch();
        return true;
    }
    return false;
}

bool SceneManager::RemoveLogic(int viewid)
{
    auto v = gTab.remove(viewid);
    if(v){
        Log::debug("注销一个房间: %d", viewid);
        v->UnLaunch();
        SAFE_DELETE(v);
        return true;
    }else{
        trace("remove error: no viewid %d", viewid);
    }
    return false;
}

void SceneManager::SendToLogic(int viewid, PacketBuffer &packet)
{
    auto v = gTab.find(viewid);
    if(v)
    {
        trace("send goto viewid ok: %d", viewid);
        v->HandlePacket(packet);
    }else{
        trace("send error: no viewid %d", viewid);
    }
}

void SceneManager::CleanLogics()
{
    Log::debug("清理所有房间");
    gTab.clear(block(GameBase* v)
    {
        v->UnLaunch();
        SAFE_DELETE(v);
    });
}

