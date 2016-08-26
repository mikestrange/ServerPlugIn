
#include "land_player.h"


LandPlayer::LandPlayer(USER_T uid, TOKEN_T tid)
:tokenid(tid)
,user_id(uid)
,seat_id(0)
{
    
}

LandPlayer::~LandPlayer(){}

bool LandPlayer::isSit()const
{
    return seat_id > 0;
}

void LandPlayer::Stand()
{
    seat_id = 0;
}

bool LandPlayer::SitDown(SEAT_T value)
{
    if(!isSit())
    {
        seat_id = value;
        return true;
    }
    return false;
}

bool LandPlayer::isSeatId(SEAT_T value)
{
    return seat_id == value;
}

SEAT_T LandPlayer::getSeatId()const
{
    return seat_id;
}

USER_T LandPlayer::getUserId()const
{
    return user_id;
}

TOKEN_T LandPlayer::getTokenId()const
{
    return tokenid;
}