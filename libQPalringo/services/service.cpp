/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as         *
 *  published by the Free Software Foundation, either version 3 of the     *
 *  License, or (at your option) any later version.                        *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU Lesser General Public License for more details.                    *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with QPalringo. If not, see                              *
 *  <http://www.gnu.org/licenses/>                                         *
 *                                                                         *
 ***************************************************************************/
#include "service.h"

Service::Service(QObject *parent)
 : QObject(parent)
{
}

void Service::loggedOn()
{
    qDebug( "Service::loggedOn - Not implemented" );
}

void Service::loginService()
{
    qDebug( "Service::loginService - Not implemented" );
}

void Service::changeOnlineStatus()
{
}

void Service::setType( qpBridgeType::Type type )
{
    this->type = type;
}

void Service::setOnlineStatus( qpOnlineStatus::OnlineStatus onlineStatus )
{
    this->onlineStatus = onlineStatus;
}

void Service::setNickname( QString nickname )
{
    this->nickname = nickname;
}

void Service::setStatus( QString status )
{
    this->status = status;
}

void Service::setId( quint32 id )
{
    this->id = id;
}


qpBridgeType::Type Service::getType() const
{
    return this->type;
}

qpOnlineStatus::OnlineStatus Service::getOnlineStatus() const
{
    return this->onlineStatus;
}

QString Service::getNickname() const
{
    return this->nickname;
}

QString Service::getStatus() const
{
    return this->status;
}

QString Service::getIcon() const
{
    return ":/svg/palringoService.svg";
}

quint32 Service::getId() const
{
    return this->id;
}

Service::~Service()
{
}
