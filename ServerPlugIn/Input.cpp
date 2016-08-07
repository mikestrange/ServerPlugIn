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
        //最多10个参数
        const char *bytes[32] = {};
        while(std::cin>>str)
        {
            int sub_len = 0;
            int prev_pos = 0;
            int length = (int)str.length();
            int argLen = 0;
            for(int i = 0; i < length; i++)
            {
                if(memcmp(&str[i], ",", 1) == 0)
                {
                    std::string byte = str.substr(prev_pos, sub_len);
                    char* dos = (char*)malloc(sub_len);
                    memcpy(dos, byte.c_str(), sub_len);
                    bytes[argLen++] = dos;
                    //
                    prev_pos = i + 1;
                    sub_len = 0;
                }else{
                    sub_len++;
                }
                if(i == length - 1 && sub_len > 0)
                {
                    std::string byte = str.substr(prev_pos, sub_len);
                    char* dos = (char*)malloc(sub_len);
                    memcpy(dos, byte.c_str(), sub_len);
                    bytes[argLen++] = dos;
                }
            }
            trace("##input = %s", str.c_str());
            if(method) method(bytes, argLen);
            for(int i = 0; i < argLen; i++)
            {
                bytes[i] = {0};
                delete bytes[i];
            }
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
        Thread::launch(&thread_complete,"Input");
    }
}