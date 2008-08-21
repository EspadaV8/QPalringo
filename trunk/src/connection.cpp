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

    qDebug("connecting to: %s as %s %s", qPrintable(this->host), qPrintable(this->emailAddress), qPrintable(this->password));

    this->conn = new QPalringoConnection( this->emailAddress, this->password, this->host, this->port );
}


Connection::~Connection()
{
}


void Connection::run()
{
    try
    {
        while( this->conn->poll() > -1 )
        {
            msleep( 42 );
        }
    }
    catch (int error)
    {
        qDebug( "error %d", error );
    }
}

void Connection::sendMessage( unsigned long long receiverID, Message *message )
{
    qDebug( "connection is sending message" );
    this->conn->sendToContact( message->Payload, message->Type, receiverID );
}

void Connection::sendToGroup( unsigned long long groupID, Message *message )
{
    qDebug( "connection is sending message" );
    this->conn->sendToGroup( message->Payload, message->Type, groupID );
}
