//
// C++ Implementation: palringoservice
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "palringoservice.h"
#include "tools.h"

PalringoService::PalringoService()
 : Service()
{
    connect( tools_, SIGNAL( connected() ), this, SLOT( loggedOn() ) );
}

void PalringoService::loggedOn()
{
    this->Nickname = tools_->user->nickname;
    this->Status = tools_->user->status;
    this->OnlineStatus = "Online";
    emit( serviceUpdated() );
}

PalringoService::~PalringoService()
{
}