//
// C++ Implementation: group
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "group.h"

Group::Group(QObject *parent)
 : Target(parent)
{
}

Group::~Group()
{
}

QString         Group::getName()
{
    return this->name;
}

QString         Group::getDescription()
{
    return this->description;
}


QSet<unsigned long long>  Group::getContacts()
{
    return this->contacts;
}

QString Group::getTitle()
{
    return this->getName();
}

QString Group::getIcon()
{
    return ":/svg/group.svg";
}

bool Group::hasContact( unsigned long long id )
{
    return this->contacts.contains( id );
}

void            Group::setName( QString name )
{
    this->name = name;
}

void            Group::setDescription( QString description )
{
    if( description == "" )
    {
        this->description = "Group Chat";
    }
    else
    {
        this->description = description;
    }
}

void            Group::setContacts( QSet<unsigned long long> contacts )
{
    this->contacts = contacts;
}
