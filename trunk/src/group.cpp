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
 : QObject(parent)
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

unsigned long long        Group::getID()
{
    return this->id;
}

QSet<unsigned long long>  Group::getContacts()
{
    return this->contacts;
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
    this->description = description;
}

void            Group::setID( unsigned long long id )
{
    this->id = id;
}

void            Group::setContacts( QSet<unsigned long long> contacts )
{
    this->contacts = contacts;
}
