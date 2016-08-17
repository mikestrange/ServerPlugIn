//
//  clients.hpp
//  ServerPlugIn
//  
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef clients_hpp
#define clients_hpp

#include "hash_map.h"
#include "lock.h"
#include "client.h"

class Client;

class Clients
{
private:
    Locked lock;
    HashMap<int, Client*> cTab;
    
public:
    bool AddClient(int fd);
    
    Client* client(int fd);
    
    Client* RemoveClient(int fd);
    
    void Clear(void(*func)(Client*));
};

#endif /* clients_hpp */
