//
//  hook_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "hook_manager.h"


STATIC_CLASS_INIT(HookManager);

HookManager::HookManager()
{
    
}

HookManager::~HookManager()
{
    CleanNodes();
}

int HookManager::AddNode(SOCKET_T sockfd, uint32 potid, int8 type)
{
    if(nodeTab.has(potid)) return -1;
    auto node = new HookNode(sockfd, potid, type);
    nodeTab.put(potid, node);
    Log::debug("注册一个挂钩 OK fd=%d, id=%d, type=%d", sockfd, potid, type);
    return 0;
}

void HookManager::DelByPotId(uint32 potid)
{
    auto node = nodeTab.remove(potid);
    if(node)
    {
        Log::debug("删除一个挂钩 rid=%d", node->getPotId());
        SAFE_DELETE(node);
    }
}

void HookManager::DelBySockFd(SOCKET_T sockfd)
{
    HashMap<uint32, HookNode*>::Iterator iter;
    for(iter = nodeTab.begin();iter!=nodeTab.end();)
    {
        HashMap<uint32, HookNode*>::Iterator miter = iter;
        iter++;
        auto node = miter->second;
        if(node->getSocketFd() == sockfd)
        {
            nodeTab.remove(miter);
            Log::debug("删除一个挂钩 rid=%d", node->getPotId());
            SAFE_DELETE(node);
        }
    }
}

void HookManager::CleanNodes()
{
    nodeTab.clear(block(HookNode* node)
                  {
                      SAFE_DELETE(node);
                  });
}

void HookManager::SendHook(uint32 potid, PacketBuffer& buff)
{
    auto node = nodeTab.find(potid);
    if(node){
        auto packet = WorldServer::getInstance()->getSocketHandler(node->getSocketFd());
        if(packet)
        {
            packet->SendPacket(&buff[0], buff.wsize());
        }else{
            Log::debug("通知挂钩Error:不存在: fd = %d", node->getSocketFd());
        }
    }else{
        Log::debug("通知挂钩Error:未注册的: potid = %d", potid);
    }
}

void HookManager::toString()
{
    Log::debug("Begin挂钩");
    nodeTab.eachMaps(block(uint32 potid, HookNode* node)
                     {
                         Log::debug("->挂钩 %d", potid);
                     });
    Log::debug("End余挂钩");
}
