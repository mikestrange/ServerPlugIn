//
//  Input.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "input.h"

static INPUT_CALL method;

static void thread_complete(Thread* thread);

static DataArray* const array = new DataArray;

static Thread* const inputLoop = Thread::launch(&thread_complete, "Input");

//class method
static void analytic(char* bytes, int size)
{
    array->clear();
    int sub_len = 0;
    int prev_pos = 0;
    for(int i = 0; i < size; i++)
    {
        if(memcmp(&bytes[i], ",", 1) == 0)
        {
            char byte[sub_len + 1];
            memcpy(byte, &bytes[prev_pos], sub_len);
            byte[sub_len] = '\0';
            array->WriteChars(byte, sub_len + 1);
            //argLen++;
            prev_pos = i + 1;
            sub_len = 0;
        }else{
            sub_len++;
        }
        //最后
        if(i == size - 1 && sub_len > 0)
        {
            char byte[sub_len + 1];
            memcpy(byte, &bytes[prev_pos], sub_len);
            byte[sub_len] = '\0';
            array->WriteChars(byte, sub_len + 1);
            //argLen++;
        }
    }
}

static void vim_complete(char* bytes, int size)
{
    //解析
    analytic(bytes, size);
    try{
        if(method) method(*array);
    }catch(Error& evt){
        Log::info("#@input handle error");
    }
    SAFE_DELETE(bytes);
}

static void thread_complete(Thread* thread)
{
    std::string str;
    while(std::cin>>str)
    {
        const int size = (int)str.length();
        char* chars = new char[size];
        memcpy(chars, str.c_str(), size);
        Log::debug("##input = %s", str.c_str());
        str.clear();
        //推送到主线程
        powder::SPushMain(&vim_complete, chars, size);
    };
    SAFE_DELETE(thread);
}

void setInputDispatcher(INPUT_CALL func)
{
    method = func;
}