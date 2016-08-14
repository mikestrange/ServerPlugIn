//
//  logic_manager.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef logic_manager_h
#define logic_manager_h

#include "global.h"
#include "logic_base.h"
#include "byte_buffer.h"
#include "packet_header.h"

#include <map>

typedef struct LogicData
{
    //唯一id
    int game_id;
    //类型
    int game_type;
    //描叙
    std::string desc;
}LogicData;

class LogicManager
{
private:
    std::map<int, LogicBase*> gTab;
public:
    LogicManager();
    virtual ~LogicManager();
    //添加一个游戏逻辑
    bool AddLogic(int gameid, LogicBase* target);
    //移除一个游戏逻辑
    bool RemoveLogic(int gameid);
    //通知(分发)
    void SendToLogic(int gameid, ReadBytes& bytes);
    //清理
    void CleanLogics();
    //处理(头)
    void HandlePacket(ReadBytes& bytes);
    
private:
    static LogicManager* _instance;
public:
    static LogicManager* getInstance();
};


#endif /* logic_manager_h */
