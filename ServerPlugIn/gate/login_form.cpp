//
//  login_form.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "login_form.h"

STATIC_CLASS_INIT(LoginForm);


int LoginForm::SubmitForm(USER_T user_id, SOCKET_T fd)
{
    if(forMap.find(user_id))
    {
        return -1;
    }
    forMap.put(user_id, fd);
    return 0;
}

int LoginForm::RemoveForm(USER_T user_id)
{
    SOCKET_T fd = forMap.remove(user_id);
    if(fd > 0)
    {
        return fd;
    }
    return -1;
}

void LoginForm::CleanForm()
{
    forMap.clear();
}