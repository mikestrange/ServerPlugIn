//
//  logic_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "logic_manager.h"


LogicManager* LogicManager::_instance = NULL;

LogicManager* LogicManager::getInstance()
{
    if(!_instance)
    {
        _instance = new LogicManager;
    }
    return _instance;
}

LogicManager::LogicManager()
{
    
}

LogicManager::~LogicManager()
{
    CleanLogics();
}

bool LogicManager::AddLogic(int gameid, LogicBase *target)
{
    std::map<int, LogicBase*>::iterator iter = gTab.find(gameid);
    if(iter == gTab.end())
    {
        gTab.insert(std::pair<int, LogicBase*>(gameid, target));
        target->launch();
        return true;
    }else{
        trace("add error: is have gameid %d delete this", gameid);
        SAFE_DELETE(target);
    }
    return false;
}

bool LogicManager::RemoveLogic(int gameid)
{
    std::map<int, LogicBase*>::iterator iter = gTab.find(gameid);
    if(iter != gTab.end())
    {
        auto logic = iter->second;
        gTab.erase(iter);
        logic->revoke();
        SAFE_DELETE(logic);
        return true;
    }else{
        trace("remove error: is no gameid %d", gameid);
    }
    return false;
}

void LogicManager::SendToLogic(int gameid, ReadBytes &bytes)
{
    std::map<int, LogicBase*>::iterator iter = gTab.find(gameid);
    if(iter != gTab.end())
    {
        iter->second->process(bytes);
    }else{
        trace("send error: is no gameid %d", gameid);
    }
}

void LogicManager::CleanLogics()
{
    std::map<int, LogicBase*>::iterator iter;
    for(iter = gTab.begin();iter != gTab.end();)
    {
        auto logic = iter->second;
        gTab.erase(iter++);
        logic->revoke();
        SAFE_DELETE(logic);
    }
    gTab.clear();
}

//头处理
void LogicManager::HandlePacket(ReadBytes &bytes)
{
    bytes.rpos(4);
    PacketHeader base(bytes);
    int32 uid;
    bytes>>uid;
    trace("房间逻辑:%d %d", uid, base.viewid);
    SendToLogic(base.viewid, bytes);
}
