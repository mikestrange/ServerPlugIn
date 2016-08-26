//
//  player.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "player.h"

Player::Player()
:token_id(0)
,user_id(0)
,sockfd(0)
,sub_sockfd(0)
,status(0)
,appid(0)
,gametype(0){}

Player::~Player(){}

void Player::WriteTo(WriteBytes& bytes)
{
    bytes<<token_id<<user_id<<sockfd<<sub_sockfd<<status;
};

void Player::ReadFor(ReadBytes& bytes)
{
    
};