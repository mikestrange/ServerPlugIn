//
//  pothook.hpp
//  ServerPlugIn
//  服务器挂钩(注册之后才能通信)
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef pothook_hpp
#define pothook_hpp

#include "log.h"
#include "global.h"
#include "network.h"
#include "hash_map.h"
#include "lock.h"
#include "potnode.h"

#include "world.h"

class PotHook
{
    STATIC_CLASS(PotHook);
private:
    HashMap<uint32, PotNode*> nodeTab;
public:
    PotHook();
    virtual ~PotHook();
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
    
    void toString();
    
public:
    
};

#endif /* pothook_hpp */
