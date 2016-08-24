//
//  ViewLaunch.hpp
//  ServerPlugIn
//  区域视图服务器
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef ViewLaunch_h
#define ViewLaunch_h

#include "ConnSocket.h"

#include "launch_base.h"
#include "packet_buffer.h"
#include "game_proxy.h"
#include "scene_manager.h"
#include "thread.h"

class SceneManager;
class GameProxy;

class GameLaunch : public LaunchBase , public ConnSocket
{
private:
    static GameLaunch* _instance;
    static void ThreadSocket(Thread* thread);
public:
    static GameLaunch* getInstance();
public:
    GameLaunch();
    virtual ~GameLaunch();
    void Launch()override;
    virtual void Launch(const char* host, int port);
    void UnLaunch()override;
public:
    void HandlePacket(PacketBuffer &packet)override;
    
private:
    const char* host;
    int port;
private:
    GameProxy* proxy;
    SceneManager* manager;
    
public:
    GameProxy* getProxy() const;
    SceneManager* getManager() const;
};

#endif /* ViewLaunch_hpp */
