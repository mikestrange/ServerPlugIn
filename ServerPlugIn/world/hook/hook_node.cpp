//
//  hook_node.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "hook_node.h"


HookNode::HookNode(SOCKET_T sockfd, uint32 potid, uint8 type)
:sockfd(sockfd)
,potid(potid)
,type(type)
{
    
}

HookNode::~HookNode()
{
    
}

SOCKET_T HookNode::getSocketFd()const
{
    return sockfd;
}

uint32 HookNode::getPotId()const
{
    return potid;
}

uint8 HookNode::getType()const
{
    return type;
}