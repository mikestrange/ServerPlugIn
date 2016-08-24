//
//  ConnSocket.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/23.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "ConnSocket.h"

ConnSocket::ConnSocket()
{
    
}

ConnSocket::~ConnSocket()
{
    
}

void ConnSocket::OnRead(char *bytes, size_t size)
{
    ByteArray* byte = ByteArrayPool::getInstance()->peel();
    if(byte == NULL)
    {
        byte = new ByteArray();
    }
    byte->copy(bytes, size);
    //read handler
    powder::RunMain(this, &ConnSocket::OnMainRead, byte);
}

void ConnSocket::OnMainRead(ByteArray* bytes)
{
    //client
    packet.LoadBytes(bytes->bytes(), bytes->size());
    try{
        while(packet.HasPacket())
        {
            packet.ReadBegin();
            //处理
            HandlePacket(packet);
            //清理缓冲
            packet.ReadEnd();
        }
    }catch(Error& evt){
        Disconnect();
    }
    //share
    if(!ByteArrayPool::getInstance()->share(bytes))
    {
        SAFE_DELETE(bytes);
    }
}

//包处理
void ConnSocket::HandlePacket(PacketBuffer &data)
{
    
}

void ConnSocket::SendPacket(PacketBuffer &buf)
{
    if(isConnect())
    {
        Send(&buf[0], buf.wsize());
    }
}