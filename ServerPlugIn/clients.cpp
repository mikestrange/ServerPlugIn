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
    if(clientTable.find(fd) != clientTable.end()) return false;
    auto client = new Client();
    client->onConnect(fd);
    clientTable.insert(std::pair<int, Client*>(fd, client));
    return true;
}

Client* Clients::client(int fd)
{
    AUTO_LOCK(&lock);
    std::map<int, Client*>::iterator iter = clientTable.find(fd);
    if(iter != clientTable.end())
    {
        return iter->second;
    }
    return NULL;
}

Client* Clients::RemoveClient(int fd)
{
    AUTO_LOCK(&lock);
    std::map<int, Client*>::iterator iter = clientTable.find(fd);
    if(iter != clientTable.end())
    {
        auto client = iter->second;
        clientTable.erase(iter);
        client->UnConnect();
        return client;
    }
    return NULL;
}

void Clients::Clear()
{
    AUTO_LOCK(&lock);
    std::map<int, Client*>::iterator iter;
    for(iter = clientTable.begin();iter != clientTable.end(); ++iter)
    {
        auto client = iter->second;
        client->UnConnect();
        SAFE_DELETE(client);
    }
    clientTable.clear();
}