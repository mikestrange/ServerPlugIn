//
//  world.cpp
//  ServerPlugIn
//  
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world.h"

#include "logininfo.h"
#include "reginfo.h"

NetServer server;
Clients clients;
//---所有消息
static WorldSession command;


static void server_accept(int fd, void* args)
{
    
}

static void server_close(int fd, void* args)
{
    Client* client = (Client*)args;
    //
    SAFE_DELETE(client);
}

static void server_read(int fd, void* args)
{
    ByteArray* byte = (ByteArray*)args;
    //client
    Client* client = clients.client(fd);
    if(client){
        client->LoadBytes(byte->bytes(), byte->size());
        try{
            while(client->HasResult())
            {
                HeadInfo header(client->packet());
                //处理
                command.perform(header.cmd, header, client);
                //
                client->flush();
            }
        }catch(Error& evt){
            server.Shut(fd);
        }
    }else{
        trace("this client is close");
    }
    //share
    if(!ByteArrayPool::getInstance()->share(byte))
    {
        SAFE_DELETE(byte);
    }
}

static void server_handler(int type, int fd, char* bytes, size_t length)
{
    if(type == SOCKET_READ)
    {
        ByteArray* byte = ByteArrayPool::getInstance()->peel();
        if(byte == NULL)
        {
            byte = new ByteArray();
        }
        byte->copy(bytes, length);
        //read handler
        powder::RunMain(Task::create(fd, &server_read, byte));
    }else if(type == SOCKET_ACCEPT){
        if(clients.AddClient(fd))
        {
            powder::RunMain(Task::create(fd, &server_accept));
        }else{
            trace("未删除的客户端: %d",fd);
            server.Shut(fd);
        }
    }else if(type == SOCKET_CLOSED){
        Client* client = clients.RemoveClient(fd);
        if(client)
        {
            //异步删除指针
            powder::RunMain(Task::create(0, &server_close, client));
        }else{
            trace("不存在的客户端: %d",fd);
        }
    }else if(type == SOCKET_SELF_CLOSED){
        //异步删除指针
        clients.Clear(block(Client* client)
        {
            powder::RunMain(Task::create(0, &server_close, client));
        });
        trace("服务器关闭");
        exit(0);
    }
}

static void thread_server(Thread* thread)
{
    if(server.Open(port)){
        int code = server.PollAttemper(&server_handler);
        trace("server close: %d", code);
    };
}


static void timer_call(int type, void* args);

static Timer* const timer = Timer::create(2, &timer_call);

static void timer_call(int type, void* args)
{
    
    trace("时间 %d",getpid());
    timer->Start();
}

static void MainRunning(int type, void* args)
{
    timer->Start();
    trace("时间 %d",getpid());
}

//---
void launch_world()
{
    powder::RunMain(&MainRunning);
    //输入
    setInputMethodAttemper(&vim);
    //
}


static void test(Thread* thread)
{
    NetSocket sock;
    ByteBuffer buffer;
    RegInfo info;
    LoginInfo info2;
    HeadInfo header;
    
//    buffer.WriteBegin();
//    header.cmd = SERVER_USER_REG;
//    buffer.WriteObject(header);
//    info.uid = 0;
//    info.appid = 102;
//    info.name = "无名";
//    info.password = "123456";
//    info.macbind = "ABCDEFGH";
//    
//    buffer.WriteObject(info);
//    buffer.WriteEnd();
//    
//    buffer.WriteBegin();
//    header.cmd = SERVER_USER_REG;
//    buffer.WriteObject(header);
//    info.uid = 0;
//    info.appid = 102;
//    info.name = "剑神SSS";
//    info.password = "123456";
//    info.macbind = "ABCDEFGH";
//    
//    buffer.WriteObject(info);
//    buffer.WriteEnd();
//    
    buffer.WriteBegin();
    header.cmd = SERVER_USER_LOGIN;
    buffer.WriteObject(header);
    info2.uid = 10000;
    info2.password = "123456";
    info2.macbind = "ABCDEFGH";
    
    buffer.WriteObject(info2);
    buffer.WriteEnd();
    if(sock.Connect("127.0.0.1", port))
    {
        sock.Send(&buffer[0], buffer.wpos());
    }
    SAFE_DELETE(thread);
}

//输入vim
void vim(int argLen, InputArray& input)
{
    if(argLen == 0) return;
    std::string str1;
    input>>str1;
    const char* byte1 = str1.c_str();
    if(strcmp(byte1, "exit") == 0)
    {
        exit(0);
    }else if(strcmp(byte1, "start") == 0 || strcmp(byte1, "run") == 0){
        Thread::launch(&thread_server, "服务器线程");
    }else if(strcmp(byte1, "stop") == 0){
        server.Shut();
    }else if(strcmp(byte1, "print") == 0){
        server.toString();
    }else if(strcmp(byte1, "open") == 0){
        Thread::launch(&test, "Socket线程");
    }
}










