//
//  landlord_logic.h
//  ServerPlugIn
//  斗地主逻辑
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef landlord_logic_h
#define landlord_logic_h

#include "logic_base.h"
#include "hash_map.h"
#include "log.h"
#include "command.h"
#include "timer.h"
#include "math_utils.h"
#include "time_utils.h"

//private
#include "landlord/land_player.h"
#include "landlord/land_seat.h"
#include "landlord/land_analyse.h"
#include "landlord/land_paper.h"
#include "landlord/land_proxy.h"

class LandPlayer;
class LandSeat;
class LandProxy;



class LandlordLogic : public LogicBase , public TimeOutEvent
{
private:
    const SEAT_T max_seat = 3;              //座位最大人数
    const int32 max_user = 200;             //房间最大人数
    const int8 hold_time = 10;              //抢装时间
    const int8 oper_time = 15;              //操作时间
private:
    int64 conduct_id;                       //游戏id
private:
    STATUS_T game_state;                    //游戏状态
private:
    SEAT_T banker_id;                       //地主id(需要游戏开始)
private:
    SEAT_T start_call_id;                   //开始叫庄位置
    SEAT_T call_id;                         //叫庄id
    int8 call_mult;                         //叫庄倍数
private:
    SEAT_T oper_id;                         //当前操作的seat_id
    TIME_T oper_begin;                      //操作开始时间
private:
    CARD_T cards[LANDLORD_CARD_LEN];        //房间牌(重复用)
    CARD_T boards[LANDLORD_SUB_CARD];       //公共牌(重复用)
private:
    Timer opTick;                           //操作计时器(重复用)
private:
    //操作
    LandPaper prev_paper;
private:
    //代理
    LandProxy* proxy;
public:
    HashMap<USER_T, LandPlayer*> allTab;    //房间所有人(需要进房间才能坐下)
    HashMap<SEAT_T, LandSeat*> seatTab;     //座位人
public:
    LandlordLogic();
    virtual ~LandlordLogic();
public:
    void Launch()override;
    void UnLaunch()override;
    void onProcess(PacketBuffer& data)override;
    void onTimeoutProcess(int type)override;
private://逻辑函数
    int StartGame();       //开始游戏，玩家准备
    void DealCards();       //发牌(玩家抢地主 30秒)
    void SetLandholder(SEAT_T bankerid);   //设置地主,给予地主3张牌，地主出牌
    void StopGame();        //游戏结束，通知胜负
    //game handle
private:
    void StartTime(delay_t delay, int type);
    //开始叫庄
    void BeginCall();
    //下一个叫庄
    void NextCall();
    //默认操作
    void DefCall();
    //开始出牌
    void BeginPlay();
    //下一个出牌
    void NextPlay();
    //默认出牌
    void DefPlay();
  //handle
public:
    int EnterLand(USER_T user_id);
    int ExitLand(USER_T user_id);
    int SitDown(USER_T user_id, SEAT_T seat_id);
    int StandUp(USER_T user_id);
    int StandBySeatId(SEAT_T seat_id);
    //出牌
    int PlayerCardAction(USER_T user_id, int8 atype, int8* nums, int8 size, CARD_TYPE ctype);
    //抢庄
    int HoldBankerAction(USER_T user_id, int8 multiple);
};

#endif /* landlord_logic_h */
