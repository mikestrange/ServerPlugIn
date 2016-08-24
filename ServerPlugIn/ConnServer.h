//
//  ConnServer.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/20.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef ConnServer_hpp
#define ConnServer_hpp

#include "server.h"
#include "hash_map.h"
#include "lock.h"
#include "main_loop.h"
#include "client.h"
#include "object_pool.h"

class Client;

class ConnServer : public NetServer
{
private:
    HashMap<int, Client*> cTab;

protected:
    virtual bool CreateClient(SockNode* conf);
    
    virtual void RemoveClient(int fd);
    
public:
    Client* getClient(int fd);
    
protected:
    virtual void OnRespond(SockNode* conf, char* bytes, size_t size)override;
    
    virtual void OnAccept(SockNode* conf)override;
    
    virtual void OnCloesd(SockNode* conf)override;
    
protected://主线程
    virtual void OnMainAccept(int fd, SockNode* conf);
    
    virtual void OnMainRead(int fd, ByteArray* bytes);
    
    virtual void OnMainClosed(int fd, SockNode* conf);
    
    virtual void HandlePacket(Client* node);
    
};

#endif /* ConnServer_hpp */
