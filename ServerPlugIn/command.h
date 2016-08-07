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
#include "headinfo.h"

class Client;

//服务端接收的消息
enum SERVER_COMMAND
{
    SERVER_USER_REG = 100,
    SERVER_USER_LOGIN,
};

//发送给客户端的消息
enum CLIENT_COMMAND
{
    CLIENT_USER_REG = 100,
    CLIENT_USER_LOGIN,
};


typedef Function(void, Client*) COMMAND_CALL;

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
        }
    }
public:
    Command(){};
    
    virtual ~Command(){}
    
    virtual void Register()=0;
    virtual void Destroy()=0;
    //是否对头处理
    virtual void HandleHeader(HeadInfo& packet)
    {
        
    };
public:
    virtual void perform(int cmd, HeadInfo& packet, Client* client)
    {
        std::map<int, COMMAND_CALL>::iterator iter = funcTable.find(cmd);
        //处理头
        HandleHeader(packet);
        //事件派发
        if(iter != funcTable.end())
        {
            trace("handle command ok cmd:%d",cmd);
            iter->second(client);
        }else{
            trace("no call cmd error:%d", cmd);
        }
    };
};

#endif /* command_hpp */
