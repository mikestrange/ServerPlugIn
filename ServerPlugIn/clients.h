//
//  clients.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef clients_hpp
#define clients_hpp

#include <stdio.h>
#include <map>

#include "lock.h"
#include "client.h"

class Client;

typedef void(*CLIENT_CALL)(Client* client);

class Clients
{
private:
    Locked lock;
    std::map<int, Client*> clientTable;
    
public:
    bool AddClient(int fd);
    
    Client* client(int fd);
    
    Client* RemoveClient(int fd);
    
    void Clear(CLIENT_CALL func);
};

#endif /* clients_hpp */
