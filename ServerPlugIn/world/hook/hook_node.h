//
//  hook_node.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef hook_node_hpp
#define hook_node_hpp

#include "global.h"
#include "network.h"

class HookNode
{
private:
    //注册ID(可以是userid,可以是gateid,可以是viewid)
    uint32 potid;
    //注册类型(POT_TYPE)
    uint8 type;
    //对应连接
    SOCKET_T sockfd;
public:
    HookNode(SOCKET_T sockfd, uint32 potid, uint8 type);
    
    virtual ~HookNode();
    
    uint32 getPotId()const;
    
    int getSocketFd()const;
    
    uint8 getType()const;
};

#endif /* hook_node_hpp */
