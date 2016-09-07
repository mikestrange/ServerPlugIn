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

#include "global.h"
#include "PowEngine.h"
#include "data_array.h"

typedef void(*INPUT_CALL)(DataArray& args);

void setInputDispatcher(INPUT_CALL func);

#endif /* Input_h */
