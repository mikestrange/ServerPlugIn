//
//  Input.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "Input.h"

#include <vector>
#include <string.h>

static INPUT_CALL method;
static InputArray putin;

static void thread_complete(Thread* thread);
//
static Thread* const inputLoop = Thread::launch(&thread_complete, "Input");

//class method
int InputArray::setInput(const char *buf)
{
    clear();
    //
    int sub_len = 0;
    int prev_pos = 0;
    int length = (int)strlen(buf);
    int argLen;
    for(int i = 0; i < length; i++)
    {
        if(memcmp(&buf[i], ",", 1) == 0)
        {
            char byte[sub_len + 1];
            memcpy(byte, &buf[prev_pos], sub_len);
            byte[sub_len] = '\0';
            WriteChars(byte, sub_len + 1);
            argLen++;
            prev_pos = i + 1;
            sub_len = 0;
        }else{
            sub_len++;
        }
        //末尾
        if(i == length - 1 && sub_len > 0)
        {
            char byte[sub_len + 1];
            memcpy(byte, &buf[prev_pos], sub_len);
            byte[sub_len] = '\0';
            WriteChars(byte, sub_len + 1);
            argLen++;
        }
    }
    return argLen;
}

static void thread_complete(Thread* thread)
{
    std::string str;
    while(std::cin>>str)
    {
        int argLen = putin.setInput(str.c_str());
        trace("##input = %s", str.c_str());
        if(method) method(argLen, putin);
        str.clear();
    };
    SAFE_DELETE(thread);
}

void setInputMethodAttemper(INPUT_CALL func)
{
    method = func;
}