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
#include "group.h"

Group::Group(QObject *parent)
 : Target(parent)
{
    this->type = Target::GROUP;
}

Group::~Group()
{
}

QString         Group::getName() const
{
    return this->name;
}

QString         Group::getDescription() const
{
    return this->description;
}


QSet<quint64>  Group::getContacts() const
{
    return this->contacts;
}

QString Group::getTitle() const
{
    return this->getName();
}

QString Group::getIcon() const
{
    return ":/svg/group.svg";
}

bool Group::hasContact( quint64 id ) const
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

void Group::addMessage( Message message )
{
    this->pendingMessages.append( message );
    emit( this->pendingMessage() );
}
