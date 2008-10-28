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


QSet<quint64>  Group::getContacts()
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

bool Group::hasContact( quint64 id )
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

void            Group::setContacts( QSet<quint64> contacts )
{
    this->contacts = contacts;
}
