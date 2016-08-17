//
//  landlord_logic.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "landlord_logic.h"


static const int START_GAME_CALL = 1;
static const int START_GAME_PLAY = 2;


LandlordLogic::LandlordLogic()
:conduct_id(0)
,game_state(LANDLORD_GAME_STOP)
,banker_id(0)
,call_id(0)
,start_call_id(0)
,call_mult(0)
,oper_id(0)
,oper_begin(0)
{
    for(int i = 0; i < LANDLORD_CARD_LEN; i++)
    {
        cards[i] = LANDLORD_CARDS[i];
    }
    proxy = new LandProxy(this);
    //test
    //StartGame();
}

LandlordLogic::~LandlordLogic()
{
    allTab.clear(block(LandPlayer* player)
    {
        SAFE_DELETE(player);
    });
    //
    seatTab.clear(block(LandSeat* seat)
    {
        SAFE_DELETE(seat);
    });
    //
    SAFE_DELETE(proxy);
}

void LandlordLogic::Launch()
{
    
}

void LandlordLogic::UnLaunch()
{
    
}

void LandlordLogic::onProcess(PacketBuffer& data)
{
    proxy->onPacketProcess(data);
}

void LandlordLogic::onTimeoutProcess(int type)
{
    if(type == START_GAME_CALL)
    {
        //下一个玩家抢(默认一个操作)
        Log::info("time out next call");
        DefCall();
        NextCall();
    }
    //
    if(type == START_GAME_PLAY)
    {
        //下一个玩家出牌(默认一个操作)
        Log::info("time out next play");
        DefPlay();
        NextPlay();
    }
}


//游戏逻辑
int LandlordLogic::StartGame()
{
    if(seatTab.size() != max_seat)
    {
        Log::warn("start error: not seat full sit size = %d", seatTab.size());
        return -1;
    }
    if(game_state != LANDLORD_GAME_STOP)
    {
        Log::warn("start error: game not stop state = %d",game_state);
        return -1;
    }
    conduct_id++;
    game_state = LANDLORD_GAME_START;
    //初始化
    banker_id = 0;
    prev_paper.init();
    //随机20次
    for(int t = 0;t < 20;t++)
    {
        for(int i = 0;i < LANDLORD_CARD_LEN; i++)
        {
            int temp = cards[i];
            int r = rand()%LANDLORD_CARD_LEN;
            cards[i] = cards[r];
            cards[r] = temp;
        }
    }
    //
    Log::info("game start:%lld",conduct_id);
    //直接发牌抢庄
    DealCards();
    return 0;
}

void LandlordLogic::DealCards()
{
    game_state = LANDLORD_ROB_BANKER;
    //玩家
    int8 floor = 0;
    for(int i = 0; i < LANDLORD_HAND_LEN; i++)
    {
        for(int p = 1; p <= max_seat; p++)
        {
            auto seat = seatTab.find(p);
            if(seat){
                seat->opens[p] = cards[floor++];
            }else{
                Log::warn("error: DealCards 座位都没人:%d", p);
            }
        }
    }
    //开始叫庄
    BeginCall();
}

void LandlordLogic::SetLandholder(SEAT_T bankerid)
{
    Log::info("deal 3 cards bank_id = %d",bankerid);
    //游戏状态
    game_state = LANDLORD_GAME_PLAYER;
    //设置庄
    oper_id = bankerid;
    banker_id = bankerid;
    //公共牌发给地主
    int8 floor = LANDLORD_CARD_LEN - LANDLORD_SUB_CARD;
    for(int8 i = 0; i < LANDLORD_SUB_CARD; i++)
    {
        auto card = cards[floor++];
        boards[i] = card;
        auto banker = seatTab.find(banker_id);
        if(banker){
            banker->opens[LANDLORD_HAND_LEN + i] = card;    //18,19,20
        }else{
            Log::info("set banker 3 cards error: not banker id =%d", banker_id);
        }
    }
    //初始化所有人的牌数
    for(int8 i = 1; i <= max_seat; i++)
    {
        auto seat = seatTab.find(i);
        if(seat){
            if(seat->seat_id == bankerid)
            {
                seat->sub_size = LANDLORD_HAND_MAX;
            }else{
                seat->sub_size = LANDLORD_HAND_LEN;
            }
        }else{
            Log::info("init seats error: not seat id = %d", i);
        }
    }
    //开始出牌
    BeginPlay();
}

//计时
void LandlordLogic::StartTime(delay_t delay, int type)
{
    oper_begin = powder::stime::gettime();
    opTick.setDelayTime(delay);
    opTick.setDelegate(this, type);
    opTick.Start();
}

int LandlordLogic::HoldBankerAction(USER_T user_id, int8 multiple)
{
    if(LANDLORD_ROB_BANKER != game_state) return -1;
    auto player = allTab.find(user_id);
    //本人操作
    if(player && player->isSeatId(oper_id))
    {
        if(call_mult > multiple)
        {
            call_id = player->getSeatId();
            call_mult = multiple;
            Log::info("call add: sid = %d, mult = %d", player->getSeatId(), call_mult);
        }else{
            Log::info("call pass: sid = %d", player->getSeatId());
        }
        NextCall();
    }
    return 0;
}

void LandlordLogic::BeginCall()
{
    //通知房间玩家给予牌, 开始抢庄
    start_call_id = Math::Random(max_seat) + 1; //1开始
    oper_id = start_call_id;
    call_id = start_call_id;
    call_mult = 0;
    //超时
    StartTime(hold_time, START_GAME_CALL);
    Log::info("begin call:%d", start_call_id);
}

void LandlordLogic::NextCall()
{
    opTick.Stop();
    if(call_mult == LANDLORD_MAX_MULT)
    {
        //达到最大倍数
        SetLandholder(call_id);
    }else{
        oper_id++;
        if(oper_id > max_seat) oper_id = 1;
        Log::info("current call:%d", oper_id);
        //轮完一圈
        if(oper_id == start_call_id)
        {
            if(call_mult == 0){
                Log::info("no call reset start");
                //设置停止
                game_state = LANDLORD_GAME_STOP;
                //重新开始吧
                StartGame();
            }else{
                SetLandholder(call_id);
            }
        }else{
            StartTime(hold_time, START_GAME_CALL);
            //通知下个玩家
        }
    }
}

void LandlordLogic::DefCall()
{
   //默认不抢
}


int LandlordLogic::PlayerCardAction(USER_T user_id, int8 atype, int8* nums, int8 size, CARD_TYPE ctype)
{
    if(LANDLORD_GAME_PLAYER != game_state) return -1;
    auto player = allTab.find(user_id);
    //本人操作
    if(player && player->isSeatId(oper_id))
    {
        auto seat = seatTab.find(oper_id);
        if(!seat)
        {
            Log::warn("oper id not user: %d", player->getSeatId());
            return -1;
        }
        //未操作(不能过牌)
        if(prev_paper.action_type == LANDLORD_OPER_NONE)
        {
            if(LANDLORD_OPER_PASS == atype)
            {
                Log::warn("can not pass oper , must open cards.");
                return -1;
            }
        }
        //过牌(下一个位置是begin_oper那么通知出牌)
        if(LANDLORD_OPER_PASS == atype)
        {
            NextPlay();
        }else{
            //跟牌/出牌 LANDLORD_OPER_FOLLOW == atype
            CARD_T gcards[size];
            for(int8 i = 0; i< size;i++)
            {
                CARD_T v = seat->opens[nums[i]];
                if(v == 0){
                    Log::warn("not card num:%d", nums[i]);
                    return -1;
                }
                gcards[i] = v;
            }
            //判断是否存在牌型
            int ret = Landlords::match(gcards, size, ctype);
            if(ret > 0)
            {
                //是否跟新本次
                if(prev_paper.outPress(gcards, size, ctype, ret))
                {
                    //更新最后的位置
                    prev_paper.oper_id = player->getSeatId();
                    //放入关闭列表
                    seat->discard(nums, size);
                    //是否出完
                    if(seat->isEmpty())
                    {
                        StopGame();
                    }else{
                        NextPlay();
                    }
                }
            }
        }
    }
    return 0;
}

void LandlordLogic::BeginPlay()
{
    prev_paper.oper_id = 0;    //无人操作
    prev_paper.action_type = LANDLORD_OPER_NONE;
    
    //超时
    StartTime(oper_time, START_GAME_PLAY);
}

//
void LandlordLogic::NextPlay()
{
    opTick.Stop();
    oper_id++;
    if(oper_id > max_seat) oper_id = 1;
    //回到自己，自己开始出牌
    if(oper_id == prev_paper.oper_id)
    {
        BeginPlay();
    }else{
        //超时
        StartTime(oper_time, START_GAME_PLAY);
        //通知下个出牌
    }
}


void LandlordLogic::DefPlay()
{
    if(prev_paper.action_type == LANDLORD_OPER_NONE)
    {
        //必须出
    }else{
        //默认Pass
    }
}

void LandlordLogic::StopGame()
{
    opTick.Stop();
    game_state = LANDLORD_GAME_OVER;
}


//非游戏流程
int LandlordLogic::EnterLand(USER_T user_id)
{
    if(allTab.size() == max_user) return -1;
    if(allTab.has(user_id)) return -1;
    //
    Log::info("Enter ok:uid = %d", user_id);
    allTab.put(user_id, new LandPlayer(user_id));
    return 0;
}

int LandlordLogic::ExitLand(USER_T user_id)
{
    auto player = allTab.remove(user_id);
    if(player)
    {
        StandBySeatId(player->getSeatId());
        Log::info("Exit room:uid = %d", user_id);
        SAFE_DELETE(player);
        return 0;
    }
    return -1;
}

int LandlordLogic::SitDown(USER_T user_id, SEAT_T seat_id)
{
    if(seat_id <= 0 || seat_id > max_seat)
    {
        Log::warn("sit down seat_id is invalid");
        return -1;
    }
    if(seatTab.size() == max_seat)
    {
        Log::warn("the game is full");
        return -1;
    }
    auto player = allTab.find(user_id);
    if(!player)
    {
        Log::warn("player not in room:uid = %d",user_id);
        return -1;
    }
    //
    if(player->SitDown(seat_id))
    {
        Log::info("sit down ok: seat_id = %d, uid = %d", seat_id, player->getUserId());
        //添加
        seatTab.put(seat_id, new LandSeat())->init(seat_id, user_id);
        //满人，游戏已经停止，立刻开始
        StartGame();
    }else{
        Log::warn("sit error: user is sit -uid = %d",user_id);
        return -1;
    }
    return 0;
}

//游戏进行中，不允许退出
int LandlordLogic::StandUp(USER_T user_id)
{
    auto player = allTab.find(user_id);
    if(player)
    {
        SEAT_T seat_id = player->getSeatId();
        player->Stand();
        return StandBySeatId(seat_id);
    }
    return -1;
}

int LandlordLogic::StandBySeatId(SEAT_T seat_id)
{
    auto seat = seatTab.remove(seat_id);
    if(seat)
    {
        Log::info("Stand: seat_id = %d", seat_id);
        SAFE_DELETE(seat);
        return 0;
    }
    return -1;
}