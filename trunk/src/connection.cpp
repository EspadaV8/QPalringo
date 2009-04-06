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
#include "connection.h"
#include "tools.h"

Connection::Connection( QString emailAddress, QString password, QString host, int port )
 : QThread()
{
    this->emailAddress = emailAddress;
    this->password = password;
    this->host = host;
    this->port = port;
    
    QString client;
    #ifdef Q_WS_WIN
        client = "x86";
    #endif
    #ifdef Q_WS_MAC
        client = "Mac";
    #endif
    #ifdef Q_WS_X11
        client = "Linux";
    #endif
    
    this->conn = new QPalringoConnection( this->emailAddress, this->password, this->host, this->port, client );
    
    connect( this, SIGNAL( disconnected() ), tools_, SLOT( disconnected() ) );
}

Connection::~Connection() { }

void Connection::run()
{
    try
    {
        while( this->conn->poll() > -1 )
        {
            msleep( 42 );
        }
        emit( disconnected() );
    }
    catch (int error)
    {
        qDebug( "error %d", error );
    }
}

void Connection::sendMessage( Target* target, Message message )
{
    this->conn->sendMessage( message.payload(), message.type(), target );
}

void Connection::getHistoryMessage( Target* target, qint32 timestamp )
{
    this->conn->getHistoryMessage( target, timestamp );
}

void Connection::joinGroup( QString groupName )
{
    this->conn->joinGroup( groupName );
}

void Connection::createGroup( QString groupName )
{
    this->conn->createGroup( groupName );
}

void Connection::leaveGroup( quint64 groupID )
{
    this->conn->leaveGroup( groupID );
}

bool Connection::updateContactDetail( QString detail, QString value )
{
    return this->conn->updateContactDetail( detail, value );
}
