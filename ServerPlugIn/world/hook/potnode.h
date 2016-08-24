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

class PotNode
{
private:
    //注册ID(可以是userid,可以是gateid,可以是viewid)
    uint32 node_id;
    //注册类型(POT_TYPE)
    uint8 regType;
    //对应连接
    int fd;
public:
    PotNode(int fd, uint32 rid, uint8 rtype);
    
    virtual ~PotNode();
    
    uint32 getNodeId()const;
    
    int getFd()const;
    
    uint8 getRegType()const;
};

#endif /* pot_node_h */
