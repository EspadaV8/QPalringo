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
    setType( "" );
    setPayload( "" );
    setSenderID( 0 );
    setGroupID( 0 );
    setSeconds( 0 );
    setUseconds( 0 );
    setBridgeID( 0 );
    setHist( false );
    setName( "Unknown" );
    setSender( 0 );
}

Message::Message( QString type, QByteArray payload, quint64 senderID, quint64 groupID,
                    quint32 seconds, quint32 useconds, quint32 bridgeID, bool hist ,
                    QString name, Target* sender )
{
    d = new MessageData;
    qRegisterMetaType<Message>("Message");

    setType( type );
    setPayload( payload );
    setSenderID( senderID );
    setGroupID( groupID );
    setSeconds( seconds );
    setUseconds( useconds );
    setBridgeID( bridgeID );
    setHist( hist );
    setName( name );
    setSender( sender );
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

void Message::setSeconds( quint32 seconds )
{
    d->seconds = seconds;
}

void Message::setUseconds( quint32 useconds )
{
    d->useconds = useconds;
}

void Message::setBridgeID( quint32 bridgeID )
{
    d->bridgeID = bridgeID;
}

void Message::setHist( bool hist )
{
    d->hist = hist;
}

void Message::setName( QString name )
{
    d->name = name;
}

void Message::setSender( Target* sender )
{
    d->sender = sender;
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

quint32 Message::seconds() const
{
    return d->seconds;
}

quint32 Message::useconds() const
{
    return d->useconds;
}

quint32 Message::bridgeID() const
{
    return d->bridgeID;
}

bool Message::hist() const
{
    return d->hist;
}

QString Message::name() const
{
    return d->name;
}

Target* Message::sender() const
{
    return d->sender;
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
