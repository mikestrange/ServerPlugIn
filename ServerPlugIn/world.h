//
//  world.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_hpp
#define world_hpp

#include <iostream>
#include "thread.h"
#include "server.h"
#include "task_scheduler.h"
#include "byte_array.h"
#include "clients.h"
#include "error.h"
#include "socket.h"
#include "timer.h"

#include "data_bank.h"

#include "Input.h"
#include "world_session.h"

#include "object_pool.h"
#include "time_utils.h"

const int port = 8001;
extern NetServer server;

void vim(const char* bytes[], int argLen);

void launch_world();

#endif /* world_hpp */
