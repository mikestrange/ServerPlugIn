//
//  clients.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "clients.h"


bool Clients::AddClient(int fd)
{
    AUTO_LOCK(&lock);
    if(cTab.has(fd)) return false;
    auto client = new Client();
    client->onConnect(fd);
    cTab.put(fd, client);
    return true;
}

Client* Clients::client(int fd)
{
    return cTab.find(fd);
}

Client* Clients::RemoveClient(int fd)
{
    AUTO_LOCK(&lock);
    auto client = cTab.remove(fd);
    if(client)
    {
        client->UnConnect();
    }
    return client;
}

void Clients::Clear(void(*func)(Client*))
{
    AUTO_LOCK(&lock);
    HashMap<int, Client*>::Iterator iter;
    for(iter = cTab.begin();iter!=cTab.end();)
    {
        auto client = iter->second;
        cTab.proto().erase(iter++);
        client->UnConnect();
        //异步指针
        func(client);
    }
}