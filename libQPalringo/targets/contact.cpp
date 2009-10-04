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
#include "contact.h"

Contact::Contact(QObject *parent)
 : Target(parent)
{
    this->type = Target::CONTACT;
}

Contact::~Contact()
{
}

void Contact::setNickname( QString nickname )
{
    this->nickname = nickname;
    emit( updateNickname() );
}

void Contact::setStatusline( QString statusline )
{
    this->statusline = statusline;
    emit( updateStatusline() );
}

void Contact::setGroup( quint64 group )
{
    this->group = group;
}

void Contact::setOnlineStatus( int onlineStatus )
{
    this->onlineStatus = onlineStatus;
    emit( updateOnlineStatus() );
}

void Contact::setBridgeType( int bridgeType )
{
    this->bridgeType = bridgeType;
}

void Contact::setDeviceType( int deviceType )
{
    this->deviceType = deviceType;
}

void Contact::setIsContact( bool isContact )
{
    this->isContact = isContact;
}

QString Contact::getTitle() const
{
    if( this->nickname.size() > 0 )
    {
        return this->nickname;
    }
    else
    {
        return "Unknown nickname";
    }
}

QString  Contact::getNickname() const
{
    return this->nickname;
}

QString  Contact::getStatusline() const
{
    if ( this->statusline == "" )
    {
        if ( this->onlineStatus > 0 )
        {
            return "Online";
        }
        else
        {
            return "Offline";
        }
    }
    return this->statusline;
}

quint64 Contact::getGroup() const
{
    return this->group;
}

int      Contact::getOnlineStatus() const
{
    return this->onlineStatus;
}

int     Contact::getBridgeType() const
{
    return this->bridgeType;
}

int     Contact::getDeviceType() const
{
    return this->deviceType;
}

bool     Contact::getIsContact() const
{
    return this->isContact;
}
