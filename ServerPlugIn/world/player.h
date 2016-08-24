//
//  player.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include "global.h"

class Player
{
public:
    TOKEN_T token_id;       //服务器识别
    USER_T user_id;         //用户id
    STATUS_T status;        //状态
    int fd;                 //入口FD
    int serverId;           //服务器id(意义不大)
    //基本信息(可以不用)
public:
    //入口id
    int appid;
    //入口类型
    int gametype;
    //平台id(没有为空)
    std::string openid;
    //机器类型
    std::string mactype;
    //机器码
    std::string macaddr;
    
public:
    Player();
    
    virtual ~Player();
    
};

#endif /* player_hpp */
