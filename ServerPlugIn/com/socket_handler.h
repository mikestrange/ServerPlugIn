//
//  socket_handler.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef socket_handler_hpp
#define socket_handler_hpp

#include "network.h"
#include "packet_buffer.h"

class Network;

class SocketHandler : public PacketBuffer
{
public:
    SocketHandler();
    SocketHandler(SOCKET_T sockt_fd);
    SocketHandler(int sock_id, SOCKET_T sockt_fd);
    
    virtual ~SocketHandler();
    
    void setHandler(SOCKET_T sockt_fd, int sock_id = 0);
    
public://on
    int handle_read(void* bytes, size_t size);
    void OnTimeout();
    
public://get
    SOCKET_T GetSocketFd();
    int GetSocketId();
    
public://close
    bool isConnect()const;
    void Disconnect();
    
public://send
    int SendPacket(const void* bytes, size_t size);
    int SendPacket(PacketBuffer& packet);
private:
    int  m_sock_id;
    SOCKET_T  m_sock_fd;
    bool is_connect;
};

#endif /* socket_handler_hpp */
