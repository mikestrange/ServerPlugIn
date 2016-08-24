//
//  net.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/9.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "net.h"

bool NET_CLOSE(int fd)
{
    if(fd > 0)
    {
        int ret = close(fd);
        trace("close fd = %d ret = %d", fd, ret);
        return ret == 0;
    }
    trace("close fd < 0(%d)", fd);
    return false;
}

int NET_RECV(int fd, void* bytes, size_t len)
{
    return (int)::recv(fd, bytes, len, 0);
}

bool NET_SEND(int fd, const void* bytes, size_t len)
{
    if(fd > 0){
        size_t ret = send(fd, bytes, len, 0);
        trace("send to fd = %d ret = %ld, len = %ld", fd, ret, len);
        return (int)ret >= 0;
    }else{
        trace("send error: (fd <= 0) fd = %d",fd);
    }
    return false;
}



