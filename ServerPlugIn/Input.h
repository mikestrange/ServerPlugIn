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

#include "byte_buffer.h"
#include "thread.h"
#include "global.h"

class InputArray;

typedef void(*INPUT_CALL)(int argLen, InputArray &input);


class InputArray : public ByteBuffer
{
public:
    int setInput(const char* buf);
};

void setInputMethodAttemper(INPUT_CALL func);

#endif /* Input_h */
