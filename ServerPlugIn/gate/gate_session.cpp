//
//  gate_session.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "gate_session.h"


STATIC_CLASS_INIT(GateSession);

void GateSession::OnPacketHandle(SOCKET_T sockfd, SocketHandler& packet)
{
    Log::info("gate message: cmd = %d, type = %d", packet.getCmd(), packet.getType());
    //类型处理
    switch(packet.getType())
    {
        case HANDLE_WORLD_MESSAGE:
                //通过socket发送
                HandleToWorld(packet);
            break;
        case HANDLE_GAME_MESSAGE:
                //通过世界发送
                HandleToWorld(packet);
            break;
        case HANDLE_HALL_MESSAGE:
                HandleToGate(packet);
            break;
        default:
                Log::debug("gate no handle message: %d", packet.getType());
            break;
    }
}

//大厅处理
void GateSession::HandleToGate(SocketHandler &packet)
{
    switch (packet.getCmd())
    {
        
    }
}

//通知世界服务器(type不能变)
void GateSession::HandleToWorld(SocketHandler& packet)
{
    //可以完全不用拷贝
    PacketBuffer buf;
    buf.CopyBegin(packet);
    buf.WriteBegin();
    packet.ReadBuffer(buf, packet.subLeng());
    buf.WriteEnd();
    GateLaunch::getInstance()->SendPacket(buf);
}

//直接通知玩家
void GateSession::HandleToPlayer(SocketHandler& packet)
{
    //可以完全不用拷贝
    PacketBuffer buf;
    buf.CopyBegin(packet);
    buf.WriteBegin();
    packet.ReadBuffer(buf, packet.subLeng());
    buf.WriteEnd();
    //players
}