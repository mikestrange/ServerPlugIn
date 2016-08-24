//
//  conn_node.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef conn_node_hpp
#define conn_node_hpp

#include "lock.h"
#include "ConnServer.h"
#include "packet_buffer.h"

class Client
{
public:
    PacketBuffer packet;
private:
    SockNode* _node;
    
public:
    Client(SockNode* node);
    
    virtual ~Client();
    
    SockNode* getSock();
};


#endif /* conn_node_hpp */
