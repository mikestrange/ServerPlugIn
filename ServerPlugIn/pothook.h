//
//  pothook.hpp
//  ServerPlugIn
//  服务器挂钩
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef pothook_hpp
#define pothook_hpp

#include "hash_map.h"
#include "pot_node.h"

//挂钩必须验证key
#define HOOK_KEY "THIS is Hook Key"

class PotHook
{
    //房间id
private:
    HashMap<int, PotNode*> nTab;
public:
    bool AddNode();
    bool RemoveNode();
};

#endif /* pothook_hpp */
