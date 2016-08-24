//
//  pot_node.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "potnode.h"

PotNode::PotNode(int fd, uint32 nid, uint8 rtype)
:fd(fd)
,node_id(nid)
,regType(rtype)
{
    
}

PotNode::~PotNode(){
    
}

USER_T PotNode::getNodeId()const
{
    return node_id;
}


int PotNode::getFd()const
{
    return fd;
}

uint8 PotNode::getRegType()const
{
    return regType;
}