//
//  ViewLaunch.hpp
//  ServerPlugIn
//  区域视图服务器
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef ViewLaunch_h
#define ViewLaunch_h

#include "global.h"
#include "base_socket.h"

#include "game_proxy.h"
#include "home_manager.h"
#include "thread.h"

class GameProxy;

class GameLaunch : public BaseSocket
{
    STATIC_CLASS(GameLaunch);
public:
    GameLaunch();
    virtual ~GameLaunch();
public:
    virtual void OnConnect()override;
    virtual void OnClose()override;
    virtual void OnProcessPacket(SOCKET_T sockfd, PacketBuffer& packet)override;
    
private:
    GameProxy* proxy;
    
public:
    GameProxy* getProxy() const;
};

#endif /* ViewLaunch_hpp */
