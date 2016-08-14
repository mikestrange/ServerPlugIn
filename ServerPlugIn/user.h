//
//  user.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/14.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef user_hpp
#define user_hpp


#include "global.h"

typedef int UID_T;
typedef long long TOKEN_T;

class User
{
public:
    TOKEN_T token_id;//服务器识别
    UID_T user_id;
    //基本信息
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
    std::string macid;
    
public:
    User(){
        
    }
    virtual ~User()
    {
        
    }
};

#endif /* user_hpp */
