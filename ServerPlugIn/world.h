//
//  world.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_h
#define world_h

#include "thread.h"
#include "net.h"
#include "task_scheduler.h"
#include "byte_array.h"
#include "clients.h"
#include "error.h"

#include "data_bank.h"

#include "Input.h"
#include "world_session.h"

#include "object_pool.h"
#include "time_utils.h"
#include "main_loop.h"

#include "Timer.h"
#include "pothook.h"
#include "logic_manager.h"


class Clients;

extern NetSocket* sockets[10];
const int port = 8001;
extern NetServer server;
extern Clients clients;

//关闭客户端
#define CLIENT_CLOSE(fd) server.Shut(fd)

void vim(int argLen, InputArray& input);

void launch_world();

#endif /* world_hpp */
