//
//  Input.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef Input_h
#define Input_h

#include <iostream>

#include "task.h"
#include "packet_buffer.h"
#include "thread.h"
#include "global.h"
#include "object_pool.h"
#include "main_loop.h"

class InputArray;

typedef void(*INPUT_CALL)(int len, InputArray& args);

class InputArray : public PacketBuffer
{
public:
    int setInput(ByteArray& byteArray);
};

void setInputMethodAttemper(INPUT_CALL func);

#endif /* Input_h */
