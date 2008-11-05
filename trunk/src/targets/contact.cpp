/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QPalringo. If not, see <http://www.gnu.org/licenses/>       *
 *                                                                         *
 ***************************************************************************/
#include "contact.h"

Contact::Contact(QObject *parent)
 : Target(parent)
{
}

Contact::~Contact()
{
}

void Contact::setNickname( QString nickname )
{
    this->nickname = nickname;
    emit( updateNickname( this->nickname ) );
}

void Contact::setStatusline( QString statusline )
{
    this->statusline = statusline;
    emit( updateStatusline( this->statusline ) );
}

void Contact::setGroup( quint64 group )
{
    this->group = group;
}

void Contact::setOnlineStatus( int onlineStatus )
{
    this->onlineStatus = onlineStatus;
    emit( updateOnlineStatus( this->onlineStatus ) );
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

QString Contact::getTitle()
{
    return this->getNickname();
}

QString  Contact::getNickname()
{
    return this->nickname;
}

QString  Contact::getStatusline()
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

QString Contact::getIcon()
{
    QString iconName;
    if ( this->getOnlineStatus() > 0 )
    {
        switch( this->getDeviceType() )
        {
            //case 1:
            //    iconName = ":/svg/botContact.svg";
            //    break;
            case 2:
                iconName = ":/svg/pcContact.svg";
                break;
            //case 3:
            //    iconName = ":/svg/mobileContact.svg";
            //    break;
            case 4:
                iconName = ":/svg/macContact.svg";
                break;
            case 5:
                iconName = ":/svg/iPhoneContact.svg";
                break;
            default:
                iconName = ":/svg/onlineContact.svg";
                break;
        }
    }
    else
    {
        iconName = ":/svg/offlineContact.svg";
    }
    return iconName;
}

quint64 Contact::getGroup()
{
    return this->group;
}

int      Contact::getOnlineStatus()
{
    return this->onlineStatus;
}

int     Contact::getBridgeType()
{
    return this->bridgeType;
}

int     Contact::getDeviceType()
{
    return this->deviceType;
}

bool     Contact::getIsContact()
{
    return this->isContact;
}