//
//  ConnServer.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/20.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "ConnServer.h"


void ConnServer::OnRespond(SockNode* conf, char* bytes, size_t size)
{
    ByteArray* byte = ByteArrayPool::getInstance()->peel();
    if(byte == NULL)
    {
        byte = new ByteArray();
    }
    byte->copy(bytes, size);
    //read handler
    powder::RunMain(this, &ConnServer::OnMainRead, conf->getFd(), byte);
};

void ConnServer::OnAccept(SockNode* conf)
{
     powder::RunMain(this, &ConnServer::OnMainAccept, conf->getFd(), conf);
};

void ConnServer::OnCloesd(SockNode* conf)
{
    powder::RunMain(this, &ConnServer::OnMainClosed, conf->getFd(),conf);
};


//main line
void ConnServer::OnMainAccept(int fd, SockNode* conf)
{
    CreateClient(conf);
}

void ConnServer::OnMainRead(int fd, ByteArray* bytes)
{
    auto node = getClient(fd);
    if(node){
        node->packet.LoadBytes(bytes->bytes(), bytes->size());
        try{
            while(node->packet.HasPacket())
            {
                node->packet.ReadBegin();
                //处理节点
                HandlePacket(node);
                //清理缓冲
                node->packet.ReadEnd();
            }
        }catch(Error& evt){
            node->getSock()->setClose();
        }
    }else{
        Log::warn("not client :%d", fd);
    }
    //share
    if(!ByteArrayPool::getInstance()->share(bytes))
    {
        SAFE_DELETE(bytes);
    }
}

void ConnServer::OnMainClosed(int fd, SockNode* conf)
{
    RemoveClient(fd);
}

//处理包
void ConnServer::HandlePacket(Client* node)
{
    
}

//self handle
bool ConnServer::CreateClient(SockNode* conf)
{
    int fd = conf->getFd();
    if(cTab.has(fd)) return false;
    auto node = new Client(conf);
    cTab.put(fd, node);
    return true;
}

Client* ConnServer::getClient(int fd)
{
    return cTab.find(fd);
}

void ConnServer::RemoveClient(int fd)
{
    auto node = cTab.remove(fd);
    SAFE_DELETE(node);
}
