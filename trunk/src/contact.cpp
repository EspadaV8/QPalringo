//
// C++ Implementation: contact
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

void Contact::setGroup( unsigned long long group )
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

unsigned long long Contact::getGroup()
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
