//
//  login_form.hpp
//  ServerPlugIn
//  提交登录数据
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef login_form_hpp
#define login_form_hpp

#include "global.h"
#include "hash_map.h"

class LoginForm
{
    STATIC_CLASS(LoginForm);
private:
    HashMap<USER_T, SOCKET_T> forMap;
public:
    //登录到服务器
    int SubmitForm(USER_T user_id, SOCKET_T fd);
    //删除后才能重新发送
    int RemoveForm(USER_T user_id);
    
    void CleanForm();
};

#endif /* login_form_hpp */
