#include "land_seat.h"

LandSeat::LandSeat(SEAT_T seatid, USER_T userid)
:seat_id(seatid)
,user_id(userid)
,card_size(0)
,out_size(0)
{
    init();
}

LandSeat::~LandSeat()
{
    
}

void LandSeat::init()
{
    for(int8 i = 0; i < LANDLORD_HAND_MAX; i++)
    {
        opens[i] = 0;
    }
    //
    SetState(LANDLORD_GAME_ONLINE);
}

void LandSeat::SetState(STATUS_T value)
{
    state = value;
}

bool LandSeat::BeState(STATUS_T value)
{
    return state == value;
}

void LandSeat::setCardSize(int8 size)
{
    card_size = size;
    out_size = 0;
}

int8 LandSeat::getCardSize() const
{
    return card_size;
}

int8 LandSeat::getSubSize()
{
    return card_size - out_size;
}
    
//放入关闭列表
bool LandSeat::Discard(int8* nums, int8 size)
{
    out_size += size;
    for(int8 i = 0; i < size; i++)
    {
        opens[nums[i]] = 0;
    }
    return out_size >= card_size;
}



