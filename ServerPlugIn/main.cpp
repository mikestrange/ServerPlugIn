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
#include "net.h"
#include "input.h"

#include "GameLaunch.h"
//src
#include "world.h"

const int port = 8001;
NetSocket* sockets[10];

static void thread_socket(Thread* thread)
{
    int index = thread->type;
    auto sock = new NetSocket();
    sockets[index] = sock;
    if(sock->Connect("127.0.0.1", port))
    {
        sock->PollAttemper();//while
    }
    //end
    SAFE_DELETE(sock);
    sockets[index] = NULL;
    SAFE_DELETE(thread);
}


//测试发送
void test_send(NetSocket* data, InputArray& input)
{
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
        buf.setHeader(SERVER_CMD_USER_LOGIN, HANDLE_WORLD_MESSAGE);
        buf.WriteBegin();
        buf.WriteObject(body);
    }else if(StringUtil::equal(str, "enter")){
        buf.setHeader(CMD_LAND_USER_ENTER, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "quit")){
        buf.setHeader(CMD_LAND_USER_EXIT, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "sit")){
        input>>str;
        SEAT_T seat_id = Basal::parseInt(str)&0xf;
        buf.setHeader(CMD_LAND_SITDOWN, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf<<seat_id;
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "stand")){
        buf.setHeader(CMD_LAND_STAND, HANDLE_GAME_MESSAGE, 0, 1002);
        buf.WriteBegin();
        buf.WriteEnd();
    }
    
    buf.WriteEnd();
    data->Send(&buf[0], buf.wpos());
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
        WorldServer::getInstance()->Launch(port);
        //
        GameLaunch::getInstance()->Launch("127.0.0.1", port);
    }else if(StringUtil::equal(str, "stop")){
        WorldServer::getInstance()->Shut();
    }else if(StringUtil::equal(str, "print")){
       WorldServer::getInstance()->toString();
    }else if(StringUtil::equal(str, "open")){
        input>>str;
        int index = Basal::parseInt(str);
        if(sockets[index]){
            trace("this socket is open: %d", index);
        }else{
            Thread::launch(&thread_socket, "NetSocket", index);
            trace("open socket: %d", index);
        }
    }else if(StringUtil::equal(str, "do")){
        input>>str;
        int index = Basal::parseInt(str);
        if(sockets[index]){
            test_send(sockets[index], input);
        }else{
            
        }
    }else if(StringUtil::equal(str, "del")){
        input>>str;
        int index = Basal::parseInt(str);
        auto sock = sockets[index];
        if(sock)
        {
            sock->Disconnect();
        }
    }else if(StringUtil::equal(str, "hook")){
        GameLaunch::getInstance()->getProxy()->HookReg(1001, 1);
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
