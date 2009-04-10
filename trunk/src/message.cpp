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
#include "message.h"

Message::Message()
{
    d = new MessageData;
    qRegisterMetaType<Message>("Message");
}

Message::Message( QString type, QByteArray payload, quint64 senderID, quint64 groupID, QDateTime timestamp,
                    quint32 seconds, quint32 useconds, bool hist )
{
    d = new MessageData;
    qRegisterMetaType<Message>("Message");

    setType( type );
    setPayload( payload );
    setSenderID( senderID );
    setGroupID( groupID );
    setTimestamp( timestamp );
    setSeconds( seconds );
    setUseconds( useconds );
    setHist( hist );
}

Message::Message( const Message &other )
    : d ( other.d )
{
}

Message::~Message()
{
}

void Message::setType( QString type )
{
    d->type = type;
}

void Message::setPayload( QByteArray payload )
{
    d->payload = payload;
}

void Message::setSenderID( quint64 senderID )
{
    d->senderID = senderID;
}

void Message::setGroupID( quint64 groupID )
{
    d->groupID = groupID;
}

void Message::setTimestamp( QDateTime timestamp )
{
    d->timestamp = timestamp;
}

void Message::setSeconds( quint32 seconds )
{
    d->seconds = seconds;
}

void Message::setUseconds( quint32 useconds )
{
    d->useconds = useconds;
}

void Message::setHist( bool hist )
{
    d->hist = hist;
}

QString Message::type() const
{
    return d->type;
}

QByteArray Message::payload() const
{
    return d->payload;
}

quint64 Message::senderID() const
{
    return d->senderID;
}

quint64 Message::groupID() const
{
    return d->groupID;
}

QDateTime Message::timestamp() const
{
    return d->timestamp;
}

quint32 Message::seconds() const
{
    return d->seconds;
}

quint32 Message::useconds() const
{
    return d->useconds;
}

bool Message::hist() const
{
    return d->hist;
}

bool Message::operator< ( const Message &other )
{
    if( d->seconds == other.seconds() )
    {
        if( d->useconds == other.useconds() )
        {
            return false;
        }
        else if( d->useconds < other.useconds() )
        {
            return true;
        }
        else if( d->useconds > other.useconds() )
        {
            return false;
        }
    }
    else if ( d->seconds < other.seconds() )
    {
        return true;
    }

    return false;
}

bool Message::operator<= ( const Message &other )
{
    if( d->seconds == other.seconds() )
    {
        if( d->useconds == other.useconds() )
        {
            return true;
        }
        else if( d->useconds < other.useconds() )
        {
            return true;
        }
        else if( d->useconds > other.useconds() )
        {
            return false;
        }
    }
    else if ( d->seconds < other.seconds() )
    {
        return true;
    }

    return false;
}

bool Message::operator> ( const Message &other )
{
    if( d->seconds == other.seconds() )
    {
        if( d->useconds == other.useconds() )
        {
            return false;
        }
        else if( d->useconds < other.useconds() )
        {
            return false;
        }
        else if( d->useconds > other.useconds() )
        {
            return true;
        }
    }
    else if ( d->seconds < other.seconds() )
    {
        return false;
    }

    return true;
}

bool Message::operator>= ( const Message &other )
{
    if( d->seconds == other.seconds() )
    {
        if( d->useconds == other.useconds() )
        {
            return true;
        }
        else if( d->useconds < other.useconds() )
        {
            return false;
        }
        else if( d->useconds > other.useconds() )
        {
            return true;
        }
    }
    else if ( d->seconds < other.seconds() )
    {
        return false;
    }

    return true;
}

bool Message::operator== ( const Message &other )
{
    if( ( d->seconds == other.seconds() ) && ( d->useconds == other.useconds() ) )
    {
        return true;
    }

    return false;
}
