//
//  pot_node.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "potnode.h"

PotNode::PotNode(SOCKET_T sockfd, uint32 potid, uint8 type)
:sockfd(sockfd)
,potid(potid)
,type(type)
{
    
}

PotNode::~PotNode(){
    
}

SOCKET_T PotNode::getSocketFd()const
{
    return sockfd;
}


uint32 PotNode::getPotId()const
{
    return potid;
}

uint8 PotNode::getType()const
{
    return type;
}