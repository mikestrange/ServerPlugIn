//
//  command.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef command_h
#define command_h

#include <stdio.h>
#include <map>

#include "client.h"
#include "packet_header.h"

class Client;

//消息类型（其实就是交给哪个服务处理）
enum MESSAGE_TYPE
{
    HANDLE_WORLD_MESSAGE = 1,
    HANDLE_GAME_MESSAGE,
    HANDLE_HALL_MESSAGE,
    HANDLE_REDIS_MESSAGE,
};

//客户端发送给服务端
enum SERVER_COMMAND
{
    SERVER_CMD_USER_REG = 100,
    SERVER_CMD_USER_LOGIN,
};

//服务端发送给客户端
enum CLIENT_COMMAND
{
    CLIENT_CMD_USER_REG = 100,
    CLIENT_CMD_USER_LOGIN,
};


typedef Function(void, PacketHeader&, Client*) COMMAND_CALL;

class Command
{
private:
    std::map<int, COMMAND_CALL> funcTable;
protected:
    void SetCommand(int cmd, COMMAND_CALL func)
    {
        if(funcTable.find(cmd) == funcTable.end()){
            funcTable.insert(std::pair<int, COMMAND_CALL>(cmd, func));
        }else{
            trace("set command error:%d",cmd);
        }
    }
    
    void UnSetCommand(int cmd)
    {
        std::map<int, COMMAND_CALL>::iterator iter = funcTable.find(cmd);
        if(iter != funcTable.end())
        {
            funcTable.erase(iter);
        }else{
            trace("this cmd is no set %d",cmd);
        }
    }
    
    void SendToCommand(int cmd, PacketHeader& data, Client* client)
    {
        std::map<int, COMMAND_CALL>::iterator iter = funcTable.find(cmd);
        //事件派发
        if(iter != funcTable.end())
        {
            trace("handle command ok cmd:%d",cmd);
            iter->second(data, client);
        }else{
            trace("no call cmd error:%d", cmd);
        }
    };
    
public:
    Command(){};
    
    virtual ~Command(){}
    
    virtual void Register()=0;
    virtual void Destroy()=0;
    //处理客户端消息
    //virtual void perform(int cmd, HeadInfo& packet, Client* client) = 0;
};

#endif /* command_hpp */
