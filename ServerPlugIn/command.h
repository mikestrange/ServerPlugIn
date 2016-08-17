//
//  command.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef command_h
#define command_h


//消息类型（其实就是交给哪个服务处理）
enum MESSAGE_TYPE
{
    HANDLE_WORLD_MESSAGE = 1,
    HANDLE_GAME_MESSAGE,
    HANDLE_HALL_MESSAGE,
    HANDLE_REDIS_MESSAGE,
};

//服务器监听
enum SERVER_READ_COMMAND
{
    SERVER_CMD_USER_REG = 100,
    SERVER_CMD_USER_LOGIN,
};

enum SERVER_LAND_READ_COMMAND
{
    CMD_LAND_USER_ENTER = 601,
    CMD_LAND_USER_EXIT,
    CMD_LAND_SITDOWN,
    CMD_LAND_STAND,
    CMD_LAND_USER_ACTION,   //跟牌或过牌
    CMD_LAND_HOLD_BANKER,   //抢庄
};

//服务器发送
enum SERVER_SEND_COMMAND
{
    CLIENT_CMD_USER_REG = 100,
    CLIENT_CMD_USER_LOGIN,
};




#endif /* command_hpp */
