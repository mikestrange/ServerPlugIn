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

static NetSocket sock;
static Clients clients;
static WorldSession command;
//
NetServer server;




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
            //server.RemoveConnection(fd);
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
        powder::PushMain(Task::create(fd, &server_read, byte));
    }else if(type == SOCKET_ACCEPT){
        if(clients.AddClient(fd))
        {
            powder::PushMain(Task::create(fd, &server_accept));
        }else{
            trace("Has been added or added more than the upper limit");
            server.closed(fd);
        }
    }else if(type == SOCKET_CLOSED){
        Client* client = clients.RemoveClient(fd);
        if(client){
            powder::PushMain(Task::create(fd, &server_close, client));
        }else{
            trace("error: Not captured to the client: %d",fd);
        }
    }else if(type == SOCKET_SELF_CLOSED){
        trace("服务器关闭");
        exit(0);
    }
}

static void thread_server(int type, Thread* thread)
{
    if(type == THREAD_OVER)
    {
        trace("server line close");
        SAFE_DELETE(thread);
    }else if(type == THREAD_BEGIN){
        trace("server line is running");
    }else{
        if(server.open(port))
        {
            server.poll(&server_handler);
        }
    }
}


//static Timer time_er;
//
//class TestDelegate : public TimeDelegate
//{
//public:
//    ~TestDelegate(){
//        trace("TestDelegate");
//    }
//public:
//    void perform(int type)override
//    {
//        time_er.start();
//    }
//    
//    void perform2(int type,int type2,int type3)
//    {
//        trace("%d %d %d",type,type2,type3);
//    }
//};
//time_er.setDelegate(new TestDelegate);
//time_er.setDelay(0);
//time_er.start();

//---
void launch_world()
{
    setInputMethod(&vim);
    
//数据库测试(连接和查找比较耗时)
//    DataBank bank;
//    bank.test_connent();
//    DataQuery query;
//    bank.find(query, "select * from user_info.player where name ='人才'");
//    query.toString();
//    query.match("name", "人才");
}

//输入vim
void vim(const char* bytes[], int argLen)
{
    for(int k = 0; k < argLen; k++)
    {
        trace("input arg%d = %s", k, bytes[k]);
    }
    const char* byte1 = bytes[0];
    if(strcmp(byte1, "exit") == 0)
    {
        exit(0);
    }else if(strcmp(byte1, "start") == 0){
        if(!server.isRunning()){
            Thread::launch(&thread_server,"服务器线程");
        }else{
            trace("is start");
        }
    }else if(strcmp(byte1, "stop") == 0){
        server.closed();
    }else if(strcmp(byte1, "print") == 0){
        server.toString();
    }else if(strcmp(byte1, "open") == 0){
        //NetSocket sock;
        ByteBuffer buffer;
        RegInfo info;
        LoginInfo info2;
        HeadInfo header;
        
//        buffer.WriteBegin();
//        header.cmd = SERVER_USER_REG;
//        buffer.WriteObject(header);
//        info.uid = 0;
//        info.appid = 102;
//        info.name = "无名";
//        info.password = "123456";
//        info.macbind = "ABCDEFGH";
//        
//        buffer.WriteObject(info);
//        buffer.WriteEnd();
//        
//        buffer.WriteBegin();
//        header.cmd = SERVER_USER_REG;
//        buffer.WriteObject(header);
//        info.uid = 0;
//        info.appid = 102;
//        info.name = "剑神SSS";
//        info.password = "123456";
//        info.macbind = "ABCDEFGH";
//        
//        buffer.WriteObject(info);
//        buffer.WriteEnd();
        
        buffer.WriteBegin();
        header.cmd = SERVER_USER_LOGIN;
        buffer.WriteObject(header);
        info2.uid = 10000;
        info2.password = "123456";
        info2.macbind = "ABCDEFGH-";
        
        buffer.WriteObject(info2);
        buffer.WriteEnd();
        
        if(sock.connect("127.0.0.1", port))
        {
            sock.SendPacket(&buffer[0], buffer.wpos());
        }
    }
}












