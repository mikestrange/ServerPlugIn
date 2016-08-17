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

NetSocket* sockets[10] = {};
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
    client->onDelete();
    SAFE_DELETE(client);
}

static void server_read(int fd, void* args)
{
    ByteArray* byte = (ByteArray*)args;
    //client
    Client* client = clients.client(fd);
    if(client){
        client->packet.LoadBytes(byte->bytes(), byte->size());
        try{
            while(client->packet.ReadBegin())
            {
                //处理
                command.HandlePacket(client);
                //清理缓冲
                client->packet.ReadEnd();
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
        server.PollAttemper(&server_handler);
    }
}

static void thread_socket(Thread* thread)
{
    int index = thread->type;
    auto sock = new NetSocket();
    sockets[index] = sock;
    if(sock->Connect("127.0.0.1", port))
    {
        sock->PollAttemper(block(int type, int fd, char* bytes, size_t size){
            
        });
    }
    //end
    SAFE_DELETE(sock);
    sockets[index] = NULL;
    SAFE_DELETE(thread);
}


//启动世界
void launch_world()
{
    powder::RunMain(block(int type, void* args)
    {
        setInputMethodAttemper(&vim);
        //
        LogicManager::getInstance();
    });
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
        buf.setHeader(CMD_LAND_USER_ENTER, HANDLE_GAME_MESSAGE);
        buf.viewid = 1002;
        buf.WriteBegin();
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "quit")){
        buf.setHeader(CMD_LAND_USER_EXIT, HANDLE_GAME_MESSAGE);
        buf.viewid = 1002;
        buf.WriteBegin();
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "sit")){
        input>>str;
        SEAT_T seat_id = Basal::parseInt(str)&0xf;
        buf.setHeader(CMD_LAND_SITDOWN, HANDLE_GAME_MESSAGE);
        buf.viewid = 1002;
        buf.WriteBegin();
        buf<<seat_id;
        buf.WriteEnd();
    }else if(StringUtil::equal(str, "stand")){
        buf.setHeader(CMD_LAND_STAND, HANDLE_GAME_MESSAGE);
        buf.viewid = 1002;
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
        Thread::launch(&thread_server, "服务器线程");
    }else if(StringUtil::equal(str, "stop")){
        server.Shut();
    }else if(StringUtil::equal(str, "print")){
        server.toString();
    }else if(StringUtil::equal(str, "new") || StringUtil::equal(str, "open")){
        input>>str;
        int index = Basal::parseInt(str);
        if(sockets[index]){
            trace("this socket is open: %d", index);
        }else{
            Thread::launch(&thread_socket, "Socket线程", index);
            trace("open socket: %d", index);
        }
    }else if(StringUtil::equal(str, "send")){
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
    }
}











