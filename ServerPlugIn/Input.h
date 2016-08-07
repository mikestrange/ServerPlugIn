//
//  Input.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef Input_h
#define Input_h

#include <stdio.h>
#include <iostream>
#include "thread.h"
#include "global.h"

typedef void(*INPUT_CALL)(const char* bytes[],int argLen);

void setInputMethod(INPUT_CALL func);

#endif /* Input_h */
