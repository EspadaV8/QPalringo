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
#include "connection.h"
#include "tools.h"

Connection::Connection( QString emailAddress, QString password, QString host, int port )
 : QThread()
{
    this->emailAddress = emailAddress;
    this->password = password;
    this->host = host;
    this->port = port;
    this->conn = new QPalringoConnection( this->emailAddress, this->password, this->host, this->port );
    
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

void Connection::sendMessage( unsigned long long receiverID, bool isGroup, Message *message )
{
    this->conn->sendMessage( message->payload, message->type, receiverID, isGroup );
}

void Connection::getHistoryMessage( qint64 target, bool isGroup, qint32 timestamp )
{
    this->conn->getHistoryMessage( target, isGroup, timestamp );
}
