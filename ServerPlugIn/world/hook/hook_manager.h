//
//  hook_manager.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef hook_manager_hpp
#define hook_manager_hpp

#include "log.h"
#include "global.h"
#include "network.h"
#include "hash_map.h"
#include "lock.h"
#include "hook_node.h"

#include "world.h"

class HookManager
{
    STATIC_CLASS(HookManager);
private:
    HashMap<uint32, HookNode*> nodeTab;
public:
    HookManager();
    virtual ~HookManager();
    //建立一个连接(返回一个tokenid > 0)
    int AddNode(SOCKET_T sockfd, uint32 potid, int8 type);
    //用户id删除
    void DelByPotId(uint32 rid);
    //fd删除
    void DelBySockFd(SOCKET_T sockfd);
    //handle
    void CleanNodes();
    //通知挂钩
    void SendHook(uint32 potid, PacketBuffer& buff);
    //
    void toString();
    
public:
    
};


#endif /* hook_manager_hpp */
