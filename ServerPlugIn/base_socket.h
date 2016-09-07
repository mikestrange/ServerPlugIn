//
//  base_socket.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef base_socket_hpp
#define base_socket_hpp

#include "network.h"
#include "PowEngine.h"
#include "memory_pool.h"

class BaseSocket : public SocketHandler, private Thread
{
protected:
    SOCKET_T listen_fd;
public:
    virtual bool connect(const char* host, int port);
    
protected:
    void run()override;
    
private:
    void on_read(void* bytes, size_t size);
    void on_main_read(char* bytes, size_t size);
public:
    virtual void OnConnect();
    virtual void OnClose();
    virtual void OnProcessPacket(SOCKET_T sockfd, PacketBuffer& packet);
};

#endif /* base_socket_hpp */
