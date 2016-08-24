//
//  landlord_logic.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "landlord_logic.h"


static const int TIMEOUT_GAME_CALL = 1;
static const int TIMEOUT_GAME_PLAY = 2;
static const int TIMEOUT_GAME_START = 3;

LandlordLogic::LandlordLogic(int32 roomid)
:view_id(roomid){
    init();
}

LandlordLogic::LandlordLogic()
:view_id(0)
{
    init();
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

void LandlordLogic::init()
{
    conduct_id = 0;
    game_state = LANDLORD_GAME_STOP;
    banker_id = 0;
    call_id = 0;
    start_call_id = 0;
    call_mult = 0;
    oper_id = 0;
    oper_begin = 0;
    //初始化牌
    for(int i = 0; i < LANDLORD_CARD_LEN; i++)
    {
        ROOM_CARDS[i] = LANDLORD_CARDS[i];
    }
    proxy = new LandProxy(this);
    //test
//    
//    EnterLand(1,101);
//    EnterLand(1,102);
//    EnterLand(1,103);
//    SitDown(101, 1);
//    SitDown(102, 2);
//    SitDown(103, 3);
//    
//    ExitLand(101);
//    ExitLand(102);
}

void LandlordLogic::Launch()
{
    
}

void LandlordLogic::UnLaunch()
{
    
}

void LandlordLogic::HandlePacket(PacketBuffer& packet)
{
    proxy->onPacketProcess(packet);
}

void LandlordLogic::onTimeoutProcess(int type)
{
    if(type == TIMEOUT_GAME_CALL)
    {
        //下一个玩家抢(默认一个操作)
        DefCall();
    }else if(type == TIMEOUT_GAME_PLAY){
        //下一个玩家出牌(默认一个操作)
        DefPlay();
    }else if(type == TIMEOUT_GAME_START){
        game_state = LANDLORD_GAME_STOP;
        StartGame();
    }
}

void LandlordLogic::ResetPlay()
{
    call_id = 0;
    start_call_id = 0;
    call_mult = 0;
    oper_id = 0;
    banker_id = 0;
    prev_paper.init();
}

//游戏逻辑
int LandlordLogic::StartGame()
{
    if(seatTab.size() != max_seat)
    {
        Log::warn("Start error: not seat full sit size = %d", seatTab.size());
        return -1;
    }
    if(game_state != LANDLORD_GAME_STOP)
    {
        Log::warn("start error: game not stop state = %d",game_state);
        return -1;
    }
    //初始化数据
    conduct_id++;
    game_state = LANDLORD_GAME_START;
    ResetPlay();
    //随机20次
    for(int t = 0; t < 20; t++)
    {
        for(int i = 0; i < LANDLORD_CARD_LEN; i++)
        {
            int temp = ROOM_CARDS[i];
            auto ran = Math::Random((int)LANDLORD_CARD_LEN);
            ROOM_CARDS[i] = ROOM_CARDS[ran];
            ROOM_CARDS[ran] = temp;
        }
    }
    Log::debug("[游戏开始] 房间ID: %d, 游戏编号:%lld", view_id, conduct_id);
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
                seat->opens[i] = ROOM_CARDS[floor++];
            }else{
                Log::warn("error: DealCards 座位都没人:%d", p);
            }
        }
    }
    //打印
    seatTab.eachMaps(block(SEAT_T k, LandSeat* seat)
    {
        std::string str = StringUtil::format("%s", Landlords::BitString(seat->opens[0]).c_str());
        for(int i = 1; i < LANDLORD_HAND_LEN; i++)
        {
            str += StringUtil::format(", %s", Landlords::BitString(seat->opens[i]).c_str());
        }
        Log::debug("uid=%d sid=%d 手牌:[%s]", seat->user_id, seat->seat_id, str.c_str());
    });
    //开始叫庄
    BeginCall();
}

void LandlordLogic::SetLandholder(SEAT_T bankerid)
{
    //游戏状态
    game_state = LANDLORD_GAME_PLAYER;
    //设置庄
    banker_id = bankerid;
    //公共牌发给地主
    int8 floor = LANDLORD_CARD_LEN - LANDLORD_SUB_CARD;
    for(int8 i = 0; i < LANDLORD_SUB_CARD; i++)
    {
        auto card = ROOM_CARDS[floor++];
        boards[i] = card;
        auto banker = seatTab.find(banker_id);
        if(banker){
            banker->opens[LANDLORD_HAND_LEN + i] = card;    //18,19,20
        }else{
            Log::info("1底牌发送失败，庄家已经离开座位 = %d", banker_id);
        }
    }
    //打印
    auto seat = seatTab.find(banker_id);
    if(seat){
        std::string str = StringUtil::format("%s", Landlords::BitString(seat->opens[LANDLORD_HAND_LEN]).c_str());
        for(int8 i = 1; i < LANDLORD_SUB_CARD; i++)
        {
            str += StringUtil::format(", %s", Landlords::BitString(seat->opens[LANDLORD_HAND_LEN + i]).c_str());
        }
        Log::info("座位玩家 %d 获得地主 倍数 %d 底牌:[%s]", bankerid, call_mult, str.c_str());
    }else{
        Log::info("2底牌发送失败，庄家已经离开座位 = %d", banker_id);
    }
    //初始化所有人的牌数
    seatTab.eachMaps([&bankerid](SEAT_T k, LandSeat* seat)
    {
        if(seat->seat_id == bankerid)
        {
            seat->setCardSize(LANDLORD_HAND_MAX);
        }else{
            seat->setCardSize(LANDLORD_HAND_LEN);
        }
    });
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

int LandlordLogic::HoldBankerAction(SEAT_T seat_id, int8 multiple)
{
    if(LANDLORD_ROB_BANKER != game_state) return -1;
    //这个时候不可能离开
    auto seat = seatTab.find(seat_id);
    //本人操作
    if(seat && seat_id == oper_id)
    {
        if(multiple > call_mult)
        {
            call_id = seat_id;
            call_mult = multiple;
            Log::info("玩家 %d 叫庄 %d倍", seat_id, call_mult);
        }else{
            Log::info("玩家 %d 不抢", seat_id);
        }
        NextCall();
    }else{
        Log::warn("[no]玩家已经离开座子: seat_id = %d",seat_id);
    }
    return 0;
}

void LandlordLogic::BeginCall()
{
    //通知房间玩家给予牌, 开始抢庄
    start_call_id = Math::Random(max_seat) + 1; //1开始
    oper_id = start_call_id;
    call_id = start_call_id;
    call_mult = 1;
    //超时
    StartTime(hold_time, TIMEOUT_GAME_CALL);
    Log::info("座位玩家:%d 开始叫庄 倍数 %d", oper_id, call_mult);
}

void LandlordLogic::NextCall()
{
    opTick.Stop();
    if(call_mult == LANDLORD_MAX_MULT)
    {
        Log::info("座位玩家:%d 下了最大倍 %d", oper_id, call_mult);
        //达到最大倍数
        SetLandholder(call_id);
    }else{
        oper_id++;
        if(oper_id > max_seat) oper_id = 1;
        //轮完一圈
        if(oper_id == start_call_id)
        {
            if(call_mult == 0){
                Log::info("无人抢庄，游戏重新开始");
                //设置停止
                game_state = LANDLORD_GAME_STOP;
                //重新开始吧
                StartGame();
            }else{
                Log::info("无人加倍 座位玩家:%d 最大倍 %d", oper_id, call_mult);
                SetLandholder(call_id);
            }
        }else{
            Log::info("座位玩家:%d 开始抢庄", oper_id);
            StartTime(hold_time, TIMEOUT_GAME_CALL);
        }
    }
}

void LandlordLogic::DefCall()
{
    //默认操作
    Log::info("座位玩家:%d 操作超时#默认操作－不抢", oper_id);
    HoldBankerAction(oper_id, 0);
}


int LandlordLogic::PlayerCardAction(SEAT_T seat_id, int8 atype, int8* nums, int8 size, CARD_TYPE ctype)
{
    if(LANDLORD_GAME_PLAYER != game_state) return -1;
    //
    auto seat = seatTab.find(seat_id);
    //本人操作
    if(seat && seat_id == oper_id)
    {
        //过牌(下一个位置是begin_oper那么通知出牌)
        if(LANDLORD_OPER_PASS == atype)
        {
            if(prev_paper.ActionType(LANDLORD_OPER_NONE))
            {
                Log::warn("本轮无人操作，不允许过牌: uid = %d", seat->user_id);
            }else{
                Log::debug("座位玩家选择过牌: %d", seat_id);
                NextPlay();
            }
        }else if(LANDLORD_OPER_FOLLOW == atype){
            //获取牌
            CARD_T gcards[size];
            for(int8 i = 0; i< size;i++)
            {
                CARD_T v = seat->opens[nums[i]];
                if(v == 0){
                    Log::warn("不存在的牌:num=%d", nums[i]);
                    return -1;
                }
                gcards[i] = v;
            }
            //打印
            if(size <= 0){
                Log::warn("玩家出牌长度为0");
                return -1;
            }else{
                
            }
            //判断是否存在牌型
            int ret = Landlords::match(gcards, size, ctype);
            if(ret > 0)
            {
                //是否跟新本次
                if(prev_paper.outPress(oper_id, gcards, size, ctype, ret))
                {
                    //打印
                    std::string str = StringUtil::format("%s", Landlords::BitString(gcards[0]).c_str());
                    for(int8 i = 1; i < size; i++)
                    {
                        str += StringUtil::format("%s", Landlords::BitString(gcards[i]).c_str());
                    }
                    int8 sub = seat->getSubSize() - size;
                    if(prev_paper.ActionType(LANDLORD_OPER_NONE)){
                        Log::debug("座位玩家出牌: %d 牌型:%d [%s] 剩余 %d", oper_id, ctype, str.c_str(), sub);
                    }else{
                        Log::debug("座位玩家跟牌: %d 牌型:%d [%s] 剩余 %d", oper_id, ctype, str.c_str(), sub);
                    }
                    //放入关闭列表 return是否出完
                    if(seat->Discard(nums, size))
                    {
                        StopGame(prev_paper.getPlayId());
                    }else{
                        NextPlay();
                    }
                    return 0;
                }else{
                    Log::warn("出牌不能大于上轮出牌");
                }
            }else{
                Log::warn("无法匹配的牌型");
            }
        }else{
            Log::debug("座位玩家操作无法识别: %d action=%d", seat_id, atype);
        }
    }else{
        Log::warn("玩家已经离开或者未轮到你操作: seat_id = %d", seat_id);
        return -1;
    }
    return -1;
}

void LandlordLogic::BeginPlay()
{
    oper_id = banker_id;
    prev_paper.init();
    Log::debug("座位玩家 %d 开始出牌", oper_id);
    //超时
    StartTime(oper_time, TIMEOUT_GAME_PLAY);
}

void LandlordLogic::NextPlay()
{
    opTick.Stop();
    oper_id++;
    if(oper_id > max_seat) oper_id = 1;
    //回到自己，自己开始出牌
    if(oper_id == prev_paper.getPlayId())
    {
        Log::debug("本轮无人跟牌, 交给 %d 操作", oper_id);
        BeginPlay();
    }else{
        Log::debug("轮到玩家 %d 出牌", oper_id);
        StartTime(oper_time, TIMEOUT_GAME_PLAY);
    }
}

void LandlordLogic::DefPlay()
{
    if(prev_paper.ActionType(LANDLORD_OPER_NONE))
    {
        auto seat = seatTab.find(oper_id);
        Log::info("等待玩家 %d 操作超时#默认出牌", oper_id);
        if(seat){
            //选择一个最小的出
            int8 size = seat->getCardSize();
            int8 subsize = seat->getSubSize();
            CARD_T* values = seat->opens;
            CARD_T cards[subsize];
            int8 f = 0;
            for(int8 i = 0; i < size; i++)
            {
                if(values[i] > 0)
                {
                    cards[f++] = values[i];
                }
            }
            //值排序
            Landlords::SortValue(cards, subsize);
            for(int8 i = 0; i < size; i++)
            {
                if(cards[0] == values[i])
                {
                    f = i;
                    break;
                }
            }
            int8 nums[1] = {f};
            //--
            PlayerCardAction(oper_id, LANDLORD_OPER_FOLLOW, nums, 1, LAND_NONE);
        }else{
            Log::info("玩家已经离开座位 %d", oper_id);
        }
    }else{
        Log::info("等待玩家 %d 操作超时#默认过牌", oper_id);
        PlayerCardAction(oper_id, LANDLORD_OPER_PASS, 0, 0, 0);
    }
}

void LandlordLogic::StopGame(SEAT_T winid)
{
    opTick.Stop();
    game_state = LANDLORD_GAME_OVER;
    if(winid == banker_id){
        Log::debug("游戏结束 庄赢");
    }else{
        Log::debug("游戏结束 农民赢");
    }
    
    //删除离开的朋友
    CleanLeaves();
    //初始化
    ResetPlay();
    //重新开始
    StartTime(over_time, TIMEOUT_GAME_START);
}


//非游戏流程
int LandlordLogic::EnterLand(TOKEN_T tid, USER_T user_id)
{
    //超过人数
    if(allTab.size() == max_user) return -1;
    //已经在房间
    if(allTab.has(user_id)) return -1;
    //
    Log::info("Enter ok:uid = %d", user_id);
    auto player = new LandPlayer(user_id, tid);
    allTab.put(user_id, player);
    //是否在座位
    for(SEAT_T i = 1; i <= max_seat; i++)
    {
        auto seat = seatTab.find(i);
        if(seat && seat->user_id == user_id)
        {
            seat->SetState(LANDLORD_GAME_ONLINE);
            player->SitDown(seat->seat_id);
            break;
        }
    }
    return 0;
}

int LandlordLogic::ExitLand(USER_T user_id)
{
    auto player = allTab.remove(user_id);
    if(!player) return -1;
    if(isPlaying()){
        auto seat = seatTab.find(player->getSeatId());
        if(seat){
            seat->SetState(LANDLORD_GAME_LEAVE);
            Log::info("Exit ok:seat set leave uid = %d seat_id = %d", user_id, seat->seat_id);
        }else{
            Log::info("Exit ok:not sit uid = %d", user_id);
        }
    }else{
        StandBySeatId(player->getSeatId());
        Log::info("Exit ok:uid = %d", user_id);
    }
    SAFE_DELETE(player);
    return 0;
}

int LandlordLogic::SitDown(USER_T user_id, SEAT_T seat_id)
{
    if(seat_id <= 0 || seat_id > max_seat)
    {
        Log::warn("Sit error: the seat_id is invalid");
        return -1;
    }
    if(seatTab.size() == max_seat)
    {
        Log::warn("Sit error: the game is full");
        return -1;
    }
    if(seatTab.has(seat_id))
    {
        Log::warn("Sit error: the seat have user: %d", seat_id);
        return -1;
    }
    auto player = allTab.find(user_id);
    if(!player)
    {
        Log::warn("Sit error: player not in room:uid = %d",user_id);
        return -1;
    }
    //成功坐下
    if(player->SitDown(seat_id))
    {
        Log::info("Sit down ok: seat_id = %d, uid = %d", seat_id, player->getUserId());
        //添加
        seatTab.put(seat_id, new LandSeat(seat_id, user_id));
        //满人，如果游戏停止，立刻开始
        StartGame();
    }else{
        Log::warn("Sit error: the user is sit uid = %d",user_id);
        return -1;
    }
    return 0;
}

//游戏进行中，不允许站起来
int LandlordLogic::StandUp(USER_T user_id)
{
    if(isPlaying())
    {
        Log::info("Stand Error: game is playing uid = %d", user_id);
        return -1;
    }
    //如果存在玩家
    auto player = allTab.find(user_id);
    if(player) player->Stand();
    //根据uid删除
    for(SEAT_T i = 1; i <= max_seat; i++)
    {
        auto seat = seatTab.find(i);
        if(seat && seat->user_id == user_id)
        {
            StandBySeatId(seat->seat_id);
            break;
        }
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

void LandlordLogic::CleanLeaves()
{
    for(SEAT_T i = 1; i <= max_seat; i++)
    {
        auto seat = seatTab.find(i);
        if(seat && seat->BeState(LANDLORD_GAME_LEAVE))
        {
            StandBySeatId(seat->seat_id);
        }
    }
}

bool LandlordLogic::isPlaying()
{
    return game_state > LANDLORD_GAME_STOP && game_state < LANDLORD_GAME_OVER;
}