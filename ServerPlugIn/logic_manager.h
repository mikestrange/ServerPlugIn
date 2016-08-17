//
//  logic_manager.h
//  ServerPlugIn
//  一个房间定义一个socket连接外部服务器
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef logic_manager_h
#define logic_manager_h

#include "global.h"
#include "logic_base.h"
#include "packet_buffer.h"
#include "hash_map.h"

#include "landlord_logic.h"

class LogicBase;
class PacketBuffer;
class LandlordLogic;

//区域数据基础
typedef struct LogicData
{
    //唯一id
    int view_id;
    //类型
    int view_type;
    //哪个服务器(哪个socket)
    int server_id;
    //描叙
    std::string desc;
}LogicData;

class LogicManager
{
private:
    HashMap<int, LogicBase*> gTab;
public:
    LogicManager();
    virtual ~LogicManager();
    //添加一个游戏逻辑
    bool AddLogic(int viewid, LogicBase* target);
    //移除一个游戏逻辑
    bool RemoveLogic(int viewid);
    //通知(分发)
    void SendToLogic(int viewid, PacketBuffer& buf);
    //清理
    void CleanLogics();
    //处理(头)
    void HandlePacket(PacketBuffer& buf);
    //发送
    void SendPacket(PacketBuffer& buf);
    
private:
    static LogicManager* _instance;
public:
    static LogicManager* getInstance();
};


#endif /* logic_manager_h */
