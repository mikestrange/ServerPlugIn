//
//  pothook.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "pothook.h"

STATIC_CLASS_INIT(PotHook);

PotHook::PotHook()
{

}

PotHook::~PotHook()
{
    CleanNodes();
}

int PotHook::AddNode(SOCKET_T sockfd, uint32 potid, int8 type)
{
    if(nodeTab.has(potid)) return -1;
    auto node = new PotNode(sockfd, potid, type);
    nodeTab.put(potid, node);
    Log::debug("注册一个挂钩 OK fd=%d, id=%d, type=%d", sockfd, potid, type);
    return 0;
}

void PotHook::DelByPotId(uint32 potid)
{
    auto node = nodeTab.remove(potid);
    if(node)
    {
        Log::debug("删除一个挂钩 rid=%d", node->getPotId());
        SAFE_DELETE(node);
    }
}

void PotHook::DelBySockFd(SOCKET_T sockfd)
{
    HashMap<USER_T, PotNode*>::Iterator iter;
    for(iter = nodeTab.begin();iter!=nodeTab.end();)
    {
        HashMap<USER_T, PotNode*>::Iterator miter = iter;
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

void PotHook::CleanNodes()
{
    nodeTab.clear(block(PotNode* node)
    {
        SAFE_DELETE(node);
    });
}

void PotHook::SendHook(uint32 potid, PacketBuffer& buff)
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

void PotHook::toString()
{
    Log::debug("Begin挂钩");
    nodeTab.eachMaps(block(uint32 potid, PotNode* node)
    {
        Log::debug("->挂钩 %d", potid);
    });
    Log::debug("End余挂钩");
}

