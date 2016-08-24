//
//  ConnSocket.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/23.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef ConnSocket_hpp
#define ConnSocket_hpp

#include "socket.h"
#include "packet_buffer.h"
#include "object_pool.h"
#include "main_loop.h"

class ConnSocket : public NetSocket
{
public:
    ConnSocket();
    virtual ~ConnSocket();
protected:
    void OnRead(char* bytes, size_t size)override;
protected:
    virtual void OnMainRead(ByteArray* bytes);
    
public:
    virtual void HandlePacket(PacketBuffer &data);
    
public:
    //发送到世界，然后分配到不同的hall_id,然后通知到client
    void SendPacket(PacketBuffer& buf);
private:
    PacketBuffer packet;
};

#endif /* ConnSocket_hpp */
