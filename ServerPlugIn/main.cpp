//
//  main.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//




//#include <iostream>
#include "server.h"
#include "socket.h"
#include "lock.h"
#include "thread.h"
#include "task_scheduler.h"
#include "object_pool.h"
#include "timer.h"
#include "packet.h"

//src
#include "world.h"

void launch(int type, void* args)
{
    
}

int main(int argc, const char * argv[])
{
    trace("init");
    //启动一个进程
    pid_t fork_id = fork();
    if(fork_id == 0){
        //子进程(登录服务器)
    }else if(fork_id > 0){
        launch_world();
        //父进程(世界服务器)
    }else{
        trace("fork error");
    }
    pthread_exit(0);
    return 0;
}
