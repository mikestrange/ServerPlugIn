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
static bool is_init = false;
static InputArray putin;

InputArray::InputArray()
{
    
}

InputArray::~InputArray()
{
    
}

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

static void thread_complete(int type, Thread* thread)
{
    if(type == THREAD_OVER)
    {
        trace("input line close");
        SAFE_DELETE(thread);
    }else if(type == THREAD_BEGIN){
        trace("input line is running");
    }else{
        std::string str;
        while(std::cin>>str)
        {
            int argLen = putin.setInput(str.c_str());
            trace("##input = %s", str.c_str());
            if(method) method(argLen, putin);
            str.clear();
        };
    }
}

void setInputMethod(INPUT_CALL func)
{
    method = func;
    if(!is_init)
    {
        is_init = true;
        Thread::launch(&thread_complete, "Input");
    }
}