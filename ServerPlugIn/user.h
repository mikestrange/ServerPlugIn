//
//  user.hpp
//  ServerPlugIn
//  用户的状态
//  Created by MikeRiy on 16/8/14.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef user_hpp
#define user_hpp


#include "global.h"

class User
{
public:
    TOKEN_T token_id;       //服务器识别
    USER_T user_id;         //用户id
    GATE_T gate_id;         //哪个大厅进入的
    STATUS_T status;        //状态
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
    User();
    
    virtual ~User();
    
public:
    bool isLogin()const;
};

#endif /* user_hpp */
