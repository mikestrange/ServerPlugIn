//
//  base_server.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef base_server_hpp
#define base_server_hpp

#include "network.h"
#include "memory_pool.h"
#include "PowEngine.h"
#include "hash_map.h"

#include "time_utils.h"

//会话代理
class ISessionProxy
{
public:
    virtual void OnPacketHandle(SOCKET_T sockfd, SocketHandler& packet)=0;
};

class BaseServer : public Network, private Thread
{
private:
    HashMap<SOCKET_T, SocketHandler*> sockMap;
protected:
    int port;
public:
    virtual ~BaseServer();
    
    bool Launch(int port);

protected:
    void run()override;
    
    //当前线程
protected:
    virtual void on_read(SocketHandler* sock, char* bytes, size_t size)override;
    
    virtual void on_register(SocketHandler* sock)override;
    
    virtual void on_close(SocketHandler* sock)override;
    
    //过渡
private:
    void on_main_read(SocketHandler* sock, char* bytes, size_t size);
    
    void on_main_register(SocketHandler* sock);
    
    void on_main_close(SocketHandler* sock);
    
    //主程序
public:
    virtual SocketHandler* getSocketHandler(SOCKET_T sockfd);
    
    virtual void OnRemove(SOCKET_T sockfd);
    
    virtual void OnRegister(SOCKET_T sockfd, SocketHandler* sock);
    
    virtual void OnProcessPacket(SOCKET_T sockfd, SocketHandler& packet);
    
    virtual void OnLaunch();
    
    virtual void OnClose();
};

#endif /* base_server_hpp */
