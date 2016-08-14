//
//  world.cpp
//  ServerPlugIn
//  
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world.h"

#include "login_body.h"
#include "reg_body.h"

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
                //处理
                command.HandlePacket(client);
                //清理缓冲
                client->flush();
            }
        }catch(Error& evt){
            client->Disconnect();
        }
    }else{
        trace("this client is close:%d", fd);
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
            CLIENT_CLOSE(fd);
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
    }
}

static void thread_server(Thread* thread)
{
    if(server.Open(port))
    {
        int code = server.PollAttemper(&server_handler);
        trace("server close: %d", code);
    };
}


//启动世界
void launch_world()
{
    powder::RunMain(block(int type, void* args)
    {
        setInputMethodAttemper(&vim);
    });
}


static void test(Thread* thread)
{
    NetSocket sock;
    //
    ByteBuffer buffer;
    LoginBody data;
    PacketHeader header;
    //
//    buffer.WriteBegin();
//    header.cmd = SERVER_CMD_USER_LOGIN;
//    header.msgType = HANDLE_WORLD_MESSAGE;
//    buffer.WriteObject(header);
//    //
//    data.uid = 10000;
//    data.password = "123456";
//    data.macbind = "ABCDEFGH-";
//    buffer.WriteObject(data);
//    buffer.WriteEnd();
    
    
    buffer.WriteBegin();
    header.cmd = SERVER_CMD_USER_LOGIN;
    header.msgType = HANDLE_GAME_MESSAGE;
    header.viewid = 1002;
    buffer.WriteObject(header);
    
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










