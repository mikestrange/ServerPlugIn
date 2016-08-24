//
//  pothook.hpp
//  ServerPlugIn
//  服务器挂钩(注册之后才能通信)
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef pothook_hpp
#define pothook_hpp

#include "hash_map.h"
#include "lock.h"

#include "potnode.h"
#include "log.h"
#include "world.h"

class PotHook
{
private:
    static PotHook* _instance;
public:
    static PotHook* getInstance();
    
private:
    //regid对应
    HashMap<uint32, PotNode*> nodeTab;
public:
    PotHook();
    virtual ~PotHook();
    //建立一个连接(返回一个tokenid > 0)
    int AddNode(int fd, uint32 rid, int8 rtype);
    //fd删除
    void DelByFd(int fd);
    //用户id删除
    void DelByNodeId(uint32 rid);
    //handle
    void CleanNodes();
    
    //通知挂钩
    void SendHook(uint32 rid, const void* bytes, size_t size);
    
    void toString();
    
public:
    
};

#endif /* pothook_hpp */
