//
//  conn_node.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "client.h"

Client::Client(SockNode* node)
:_node(node)
{
    
}

Client::~Client()
{
    trace("~Client %d", _node->getFd());
    SAFE_DELETE(_node);
}


SockNode* Client::getSock()
{
    return _node;
}
