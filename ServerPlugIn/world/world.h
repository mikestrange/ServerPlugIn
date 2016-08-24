//
//  world.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_h
#define world_h

#include "ConnServer.h"

#include "thread.h"
#include "error.h"

#include "object_pool.h"
#include "main_loop.h"

#include "world_session.h"
#include "launch_base.h"

#include "pothook.h"


class WorldSession;
class PotHook;

class WorldServer : public LaunchBase, public ConnServer
{
private:
    static WorldServer* _instance;
public:
    static WorldServer* getInstance();
private:
    static void ThreadServer(Thread* thread);
private:
    int port;
    WorldSession* session;
public:
    WorldServer();
    virtual ~WorldServer();
    void Launch()override;
    virtual void Launch(int port);
    void UnLaunch()override;
protected:
    void HandlePacket(Client* node)override;
    
    virtual void OnMainClosed(int fd, SockNode* conf)override;
    
public:
    WorldSession* getSession()const;
};

#endif /* world_hpp */
