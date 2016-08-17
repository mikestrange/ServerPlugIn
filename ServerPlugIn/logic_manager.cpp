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
    //init
    AddLogic(1002, new LandlordLogic);
}

LogicManager::~LogicManager()
{
    CleanLogics();
}

bool LogicManager::AddLogic(int viewid, LogicBase *target)
{
    if(gTab.has(viewid)){
        trace("add error: have viewid [%d] delete this", viewid);
        SAFE_DELETE(target);
    }else{
        gTab.put(viewid, target);
        target->Launch();
        return true;
    }
    return false;
}

bool LogicManager::RemoveLogic(int viewid)
{
    auto v = gTab.remove(viewid);
    if(v){
        v->UnLaunch();
        SAFE_DELETE(v);
        return true;
    }else{
        trace("remove error: no viewid %d", viewid);
    }
    return false;
}

void LogicManager::SendToLogic(int viewid, PacketBuffer &buf)
{
    auto v = gTab.find(viewid);
    if(v)
    {
        trace("send goto viewid ok: %d", viewid);
        v->onProcess(buf);
    }else{
        trace("send error: no viewid %d", viewid);
    }
}

void LogicManager::CleanLogics()
{
    gTab.clear(block(LogicBase* v)
    {
        v->UnLaunch();
        SAFE_DELETE(v);
    });
}

//头处理
void LogicManager::HandlePacket(PacketBuffer &buf)
{
    buf.ReadBegin();
    //分发到各个房间
    SendToLogic(buf.viewid, buf);
    //读结束
    buf.ReadEnd();
}


void LogicManager::SendPacket(PacketBuffer &buf)
{
    //sock.Send(&buf[0], buf());
}
