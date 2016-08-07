//
//  net_base.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "net_base.h"

static Locked close_lock;
static Locked send_lock;

namespace net
{
    bool close_fd(int fd)
    {
        if(fd > 0)
        {
            AUTO_LOCK(&close_lock);
            int ret = close(fd);
            trace("close fd = %d ret = %d", fd, ret);
            return ret == 0;
        }
        trace("close fd < 0(%d)", fd);
        return false;
    }
    
    int recv_fd(int fd, void* bytes, size_t len)
    {
        return (int)::recv(fd, bytes, len, 0);
    }
    
    bool send_fd(int fd, const void* bytes, size_t len)
    {
        if(fd > 0){
            AUTO_LOCK(&send_lock);
            size_t ret = send(fd, bytes, len, 0);
            trace("send to fd = %d ret = %ld, len = %ld", fd, ret, len);
            return (int)ret >= 0;
        }else{
            trace("send error: (fd <= 0) fd = %d",fd);
        }
        return false;
    }
}