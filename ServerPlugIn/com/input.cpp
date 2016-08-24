//
//  Input.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "input.h"

static INPUT_CALL method;
static InputArray putin;

static void thread_complete(Thread* thread);
//
static Thread* const inputLoop = Thread::launch(&thread_complete, "Input");

//class method
int InputArray::setInput(ByteArray& byteArray)
{
    clear();
    int sub_len = 0;
    int prev_pos = 0;
    int length = (int)byteArray.size();
    const char* buf = byteArray.bytes();
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

static void vim_complete(ByteArray* byte)
{
    //解析
    int size = putin.setInput(*byte);
    //推送输出
    try{
        if(method) method(size, putin);
    }catch(Error& evt){
        trace("#@input handle error");
    }
    //share
    if(!ByteArrayPool::getInstance()->share(byte))
    {
        SAFE_DELETE(byte);
    }
}

static void thread_complete(Thread* thread)
{
    std::string str;
    while(std::cin>>str)
    {
        ByteArray* byte = ByteArrayPool::getInstance()->peel();
        if(byte == NULL)
        {
            byte = new ByteArray();
        }
        byte->copy(str.c_str(), str.length());
        trace("##input = %s", str.c_str());
        vim_complete(byte);
        //powder::SRunMain(&vim_complete, byte);
        str.clear();
    };
    SAFE_DELETE(thread);
}

void setInputMethodAttemper(INPUT_CALL func)
{
    method = func;
}