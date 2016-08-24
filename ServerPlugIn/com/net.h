//
//  net.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/9.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef net_h
#define net_h

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
#include "time_utils.h"
#include "memory_pool.h"

class FdList;
class NetServer;
class NetSocket;

//最大读取缓冲
#define MAX_BUFFER 1024

//socket回执方法
//typedef void(*SCOKET_CALL)(int type, int fd, char* bytes, size_t length);

//socket回执事件
typedef enum _SOCKET_EVENT
{
    SOCKET_ACCEPT = 1,
    SOCKET_READ,
    SOCKET_CLOSED,        //被动关闭(客户端)
    SOCKET_SELF_CLOSED,   //self close
}SOCKET_EVENT;


//公用
bool NET_CLOSE(int fd);

int NET_RECV(int fd, void* bytes, size_t len);

bool NET_SEND(int fd, const void* bytes, size_t len);

//服务器





#endif /* net_hpp */
