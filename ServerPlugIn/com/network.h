//
//  network.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/24.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef network_hpp
#define network_hpp

#define INVALID_SOCKET -1

#define MAX_LISTENE 10

#define MAX_SERVER 1024

#define MAX_BUFFER 1024

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "arpa/inet.h"

#include <errno.h>
#include <fcntl.h>
#include <err.h>

#include "global.h"
#include "log.h"
#include "socket_handler.h"
#include "memory_pool.h"

class SocketHandler;


#define SET_TIMEOUT(t,m,s) t.tv_sec = m;t.tv_usec = s;

class Network
{
protected:
    SOCKET_T listen_fd;
private:
    int maxfds;
    bool isrunning;
private:
    fd_set m_rset;
    struct sockaddr_in server_addr;
    SocketHandler* sock_list[MAX_SERVER];
    
public:
    Network();
    
    virtual ~Network();
    
    virtual bool create_listener(int port);
    
    virtual int select_server();
    
    virtual void stop_server();
    
    virtual void toString();
    
public:
    static SOCKET_T connect_server(const char* ip, int port);
    
private:
    SocketHandler* alloc_sockethandler(int index, SOCKET_T fd);
    
    void register_event(SOCKET_T fd);
    
    void handle_close(int index);
    
    void handle_accept();
    
protected:
    virtual void on_read(SocketHandler* sock, char* bytes, size_t size);
    
    virtual void on_register(SocketHandler* sock);
    
    virtual void on_close(SocketHandler* sock);
    
public:
    static bool NET_CLOSE(SOCKET_T fd);
    
    static int NET_RECV(SOCKET_T fd, void* bytes, size_t len);
    
    static int NET_SEND(SOCKET_T fd, const void* bytes, size_t len);
};

#endif /* network_hpp */
