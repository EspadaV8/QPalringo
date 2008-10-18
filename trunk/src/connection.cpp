//
// C++ Implementation: connection
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
