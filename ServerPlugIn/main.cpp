//
//  main.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//


#include "lock.h"
#include "thread.h"
#include "task_scheduler.h"
#include "object_pool.h"

#include "main_loop.h"
#include "input.h"

#include "GameLaunch.h"
#include "GateLaunch.h"
//src
#include "world.h"
#include "gate.h"

const int world_port = 8001;
const int gate_port = 8002;

void run_server(InputArray& input)
{
    std::string str;
    input>>str;
    if(StringUtil::equal(str, "world"))
    {
        WorldServer::getInstance()->Launch(world_port);
    }else if(StringUtil::equal(str, "gate")){
        GateServer::getInstance()->Launch(gate_port);
    }else if(StringUtil::equal(str, "game_to_world")){
        GameLaunch::getInstance()->connect("127.0.0.1", world_port);
    }else if(StringUtil::equal(str, "gate_to_world")){
        GateLaunch::getInstance()->connect("127.0.0.1", world_port);
    }
}

void stop_server(InputArray& input)
{
    std::string str;
    input>>str;
    if(StringUtil::equal(str, "world"))
    {
        WorldServer::getInstance()->stop_server();
    }else if(StringUtil::equal(str, "gate")){
        GateServer::getInstance()->stop_server();
    }else if(StringUtil::equal(str, "game_to_world")){
        GameLaunch::getInstance()->Disconnect();
    }else if(StringUtil::equal(str, "gate_to_world")){
        GateLaunch::getInstance()->Disconnect();
    }
}

BaseSocket m_socket;

//测试发送
void open_send(InputArray& input)
{
    
    m_socket.connect("127.0.0.1", world_port);
    std::string str;
    input>>str;
    PacketBuffer buf;
    if(StringUtil::equal(str, "login"))
    {
        input>>str;
        USER_T user_id = Basal::parseInt(str);
        LoginBody body;
        body.uid = user_id;//唯一写入
        body.password = "123456";
        body.macbind = "ABCDEFGH";
        buf.setBegin(SERVER_CMD_USER_LOGIN, HANDLE_WORLD_MESSAGE);
        buf.WriteBegin();
        buf.WriteObject(body);
    }else if(StringUtil::equal(str, "enter")){
        buf.setBegin(CMD_LAND_USER_ENTER, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "quit")){
        buf.setBegin(CMD_LAND_USER_EXIT, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "sit")){
        input>>str;
        SEAT_T seat_id = Basal::parseInt(str)&0xf;
        buf.setBegin(CMD_LAND_SITDOWN, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf<<seat_id;
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "stand")){
        buf.setBegin(CMD_LAND_STAND, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf.WriteEnd();
    }
    buf.WriteEnd();
    //
    m_socket.SendPacket(buf);
}

//输入vim
void vim(int argLen, InputArray& input)
{
    std::string str;
    input>>str;
    if(StringUtil::equal(str, "exit"))
    {
        exit(0);
    }else if(StringUtil::equal(str, "run")){
        run_server(input);
    }else if(StringUtil::equal(str, "stop")){
        stop_server(input);
    }else if(StringUtil::equal(str, "print")){
       WorldServer::getInstance()->toString();
    }else if(StringUtil::equal(str, "send")){
        open_send(input);
    }else if(StringUtil::equal(str, "hook")){
        GameLaunch::getInstance()->getProxy()->ToHookReg(1001, 1);
    }
}


int main(int argc, const char * argv[])
{
    Log::debug("====START WORLD====");
    powder::SRunMain(block()
                     {
                         setInputMethodAttemper(&vim);
                     });
    
    pthread_exit(0);
    return 0;
}
