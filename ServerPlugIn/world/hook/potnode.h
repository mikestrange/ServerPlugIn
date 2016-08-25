//
//  pot_node.h
//  ServerPlugIn
//  钩子数据
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef pot_node_h
#define pot_node_h

#include "global.h"
#include "network.h"

class PotNode
{
private:
    //注册ID(可以是userid,可以是gateid,可以是viewid)
    uint32 potid;
    //注册类型(POT_TYPE)
    uint8 type;
    //对应连接
    SOCKET_T sockfd;
public:
    PotNode(SOCKET_T sockfd, uint32 potid, uint8 type);
    
    virtual ~PotNode();
    
    uint32 getPotId()const;
    
    int getSocketFd()const;
    
    uint8 getType()const;
};

#endif /* pot_node_h */
