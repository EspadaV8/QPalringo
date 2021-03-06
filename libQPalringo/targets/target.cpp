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
#include "target.h"
#include <QDebug>

Target::Target(QObject *parent)
 : QObject(parent)
{
}

Target::~Target()
{
}

quint64 Target::getID() const
{
    return this->id;
}

void Target::setID( quint64 id )
{
    this->id = id;
}

QString Target::getTitle() const
{
    return "QPalringo";
}

int Target::getType()
{
    return this->type;
}

void Target::addMessage( Message message )
{
    this->pendingMessages.append( message );
    emit pendingMessage( this );
}

void Target::clearPending()
{
    this->pendingMessages.clear();
    emit( this->clearedPendingMessages() );
}

QList<Message> Target::getPendingMessages()
{
    return this->pendingMessages;
}
