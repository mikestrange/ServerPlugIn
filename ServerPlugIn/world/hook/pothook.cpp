//
//  pothook.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "pothook.h"

PotHook* PotHook::_instance = NULL;

PotHook* PotHook::getInstance()
{
    if(!_instance)
    {
        _instance = new PotHook;
    }
    return _instance;
}

PotHook::PotHook()
{

}

PotHook::~PotHook()
{
    CleanNodes();
}

int PotHook::AddNode(int fd, uint32 nid, int8 rtype)
{
    if(nodeTab.has(nid)) return -1;
    auto node = new PotNode(fd, nid, rtype);
    nodeTab.put(nid, node);
    Log::debug("注册一个挂钩 OK fd=%d, rid=%d, rt=%d", fd, nid, rtype);
    return 0;
}

void PotHook::DelByNodeId(uint32 nid)
{
    auto node = nodeTab.remove(nid);
    if(node)
    {
        Log::debug("删除一个挂钩 rid=%d", node->getNodeId());
        SAFE_DELETE(node);
    }
}

void PotHook::DelByFd(int fd)
{
    HashMap<USER_T, PotNode*>::Iterator iter;
    for(iter = nodeTab.begin();iter!=nodeTab.end();)
    {
        HashMap<USER_T, PotNode*>::Iterator miter = iter;
        iter++;
        auto node = miter->second;
        if(node->getFd() == fd)
        {
            nodeTab.remove(miter);
            Log::debug("删除一个挂钩 rid=%d", node->getNodeId());
            SAFE_DELETE(node);
        }
    }
}

//清理所有
void PotHook::CleanNodes()
{
    nodeTab.clear(block(PotNode* node)
    {
        SAFE_DELETE(node);
    });
}


void PotHook::SendHook(uint32 rid, const void* bytes, size_t size)
{
    auto node = nodeTab.find(rid);
    if(node){
        auto client = WorldServer::getInstance()->getClient(node->getFd());
        if(client)
        {
            client->getSock()->Send(bytes, size);
        }else{
            Log::debug("通知单元Error:不存在的fd客户端");
        }
    }else{
        Log::debug("通知单元Error:未注册的单元 %d",rid);
    }
}

void PotHook::toString()
{
    Log::debug("Begin挂钩");
    nodeTab.eachMaps(block(uint32 uid, PotNode* node)
    {
        Log::debug("->挂钩 %d",uid);
    });
    Log::debug("End余挂钩");
}

