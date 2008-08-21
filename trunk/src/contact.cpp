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
}

void Contact::setID( unsigned long long id )
{
    this->id = id;
}

void Contact::setGroup( unsigned long long group )
{
    this->group = group;
}

void Contact::setOnlineStatus( int onlineStatus )
{
    this->onlineStatus = onlineStatus;
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



QString  Contact::getNickname()
{
    return this->nickname;
}

QString  Contact::getStatusline()
{
    return this->statusline;
}

unsigned long long Contact::getID()
{
    return this->id;
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
