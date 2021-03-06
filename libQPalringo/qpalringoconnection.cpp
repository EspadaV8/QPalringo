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
#include <QHash>
#include <QMap>
#include <QSet>
#include "qpalringoconnection.h"

// the max packet size we can send
#define MAX_PACKET_SIZE 512

#define SIGNALS 0
#define PARSING 0
#define ERRORS 1
#define qpDEBUG 0
#define INFO 0

QPalringoConnection::QPalringoConnection(QString login,
                                         QString password,
                                         QString clientType,
                                         QString host,
                                         quint16 port,
                                         qint32 protocol )
{
    this->host = host;
    this->port = port;
    this->clientType = clientType;

    this->protocolVersion_ = protocol;
    this->compression_ = 0;
    this->packetSeq_ = 0;
    this->receivedData_ = 0;
    this->outMessageCount_ = 0;
    this->mesg_id_ = 0;
    this->encryption_ = false;

    this->user.email = login;
    this->user.password = password;

    initOutSignals();
    initInSignals();
}

void QPalringoConnection::initOutSignals()
{
    outSignals.insert( qpCommand::PING, "pingSent" );
    outSignals.insert( qpCommand::LOGON, "logonSent" );
    outSignals.insert( qpCommand::BYE, "byeSent" );
    outSignals.insert( qpCommand::AUTH, "authSent" );
    outSignals.insert( qpCommand::CONTACT_UPDATE, "contactUpdateSent" );
    outSignals.insert( qpCommand::CONTACT_ADD_RESP, "contactAddRespSent" );
    outSignals.insert( qpCommand::GROUP_SUBSCRIBE, "groupSubscribeSent" );
    outSignals.insert( qpCommand::GROUP_UNSUB, "groupUnsubSent" );
    outSignals.insert( qpCommand::GROUP_CREATE, "groupCreateSent" );
    outSignals.insert( qpCommand::GROUP_INVITE, "groupInviteSent" );
    outSignals.insert( qpCommand::GROUP_ADMIN, "groupAdminSent" );
    outSignals.insert( qpCommand::MESG, "mesgSent" );
    outSignals.insert( qpCommand::MESG_STORED, "mesgStoredSent" );
    outSignals.insert( qpCommand::MESG_HIST, "mesgHistSent" );
    outSignals.insert( qpCommand::REG, "regSent" );

    connect( this, SIGNAL( pingSent( Headers, QByteArray ) ),
             this, SLOT( onPingSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( logonSent( Headers, QByteArray ) ),
             this, SLOT( onLogonSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( byeSent( Headers, QByteArray ) ),
             this, SLOT( onByeSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( authSent( Headers, QByteArray ) ),
             this, SLOT( onAuthSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( contactUpdateSent( Headers, QByteArray ) ),
             this, SLOT( onContactUpdateSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( contactAddRespSent( Headers, QByteArray ) ),
             this, SLOT( onContactAddRespSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( groupSubscribeSent( Headers, QByteArray ) ),
             this, SLOT( onGroupSubscribeSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( groupUnsubSent( Headers, QByteArray ) ),
             this, SLOT( onGroupUnsubSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( groupCreateSent( Headers, QByteArray ) ),
             this, SLOT( onGroupCreateSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( groupInviteSent( Headers, QByteArray ) ),
             this, SLOT( onGroupInviteSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( groupAdminSent( Headers, QByteArray ) ),
             this, SLOT( onGroupAdminSent ( Headers, QByteArray ) ) );
    connect( this, SIGNAL( mesgSent( Headers, QByteArray ) ),
             this, SLOT( onMesgSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( mesgStoredSent( Headers, QByteArray ) ),
             this, SLOT( onMesgStoredSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( mesgHistSent( Headers, QByteArray ) ),
             this, SLOT( onMesgHistSent( Headers, QByteArray ) ) );
    connect( this, SIGNAL( regSent( Headers, QByteArray ) ),
             this, SLOT( onRegSent( Headers, QByteArray ) ) );
}

void QPalringoConnection::initInSignals()
{
    inSignals.insert( qpCommand::AUTH, "authRecieved" );
    inSignals.insert( qpCommand::LOGON_SUCCESSFUL, "logonSuccessfulRecieved" );

    inSignals.insert( qpCommand::MESG, "mesgRecieved" );
    inSignals.insert( qpCommand::PING, "pingRecieved" );
    inSignals.insert( qpCommand::RESPONSE, "responseRecieved" );
    inSignals.insert( qpCommand::BRIDGE, "bridgeRecieved" );
    inSignals.insert( qpCommand::BRIDGE_MESG, "bridgeMesgRecieved" );
    inSignals.insert( qpCommand::BRIDGE_ON, "bridgeOnRecieved" );


    connect( this, SIGNAL( authRecieved( const Headers, const QByteArray ) ),
             this, SLOT( onAuthRecieved( const Headers, const QByteArray ) ) );
    connect( this, SIGNAL( logonSuccessfulRecieved( const Headers, const QByteArray ) ),
             this, SLOT( onLogonSuccessfulReceived( const Headers, const QByteArray ) ) );
    connect( this, SIGNAL( mesgRecieved( const Headers, const QByteArray ) ),
             this, SLOT( onMesgReceived( const Headers, const QByteArray ) ) );
    connect( this, SIGNAL( pingRecieved( const Headers, const QByteArray ) ),
             this, SLOT( onPingReceived( const Headers, const QByteArray ) ) );
    connect( this, SIGNAL( responseRecieved( const Headers, const QByteArray ) ),
             this, SLOT( onResponseReceived( const Headers, const QByteArray ) ) );
    connect( this, SIGNAL( bridgeMesgRecieved( const Headers, const QByteArray ) ),
             this, SLOT( onMesgReceived( const Headers, const QByteArray ) ) );
    connect( this, SIGNAL( bridgeOnRecieved( const Headers, const QByteArray ) ),
            this, SLOT( onBridgeOnReceived( const Headers, const QByteArray ) ) );

    if( this->protocolVersion_ == 1 )
    {
        inSignals.insert( qpCommand::CONTACT_DETAIL, "contactDetailRecieved" );
        inSignals.insert( qpCommand::GROUP_DETAIL, "groupDetailRecieved" );
        inSignals.insert( qpCommand::BRIDGE_CONTACT, "bridgeContactRecieved" );

        connect( this, SIGNAL( contactDetailRecieved( const Headers, const QByteArray ) ),
                 this, SLOT( onContactDetailReceived( const Headers, const QByteArray ) ) );
        connect( this, SIGNAL( groupDetailRecieved( const Headers, const QByteArray ) ),
                 this, SLOT( onGroupDetailReceived( const Headers, const QByteArray ) ) );
        connect( this, SIGNAL( bridgeRecieved( const Headers, const QByteArray ) ),
                 this, SLOT( onBridgeReceived( const Headers, const QByteArray ) ) );
        connect( this, SIGNAL( bridgeContactRecieved( const Headers, const QByteArray ) ),
                 this, SLOT( onBridgeContactReceived( const Headers, const QByteArray ) ) );
    }
    else if( this->protocolVersion_ == 2 )
    {
        inSignals.insert( qpCommand::SUB_PROFILE, "subProfileReceived" );
        inSignals.insert( qpCommand::BRIDGE_CONTACT, "bridgeContactRecieved" );
        connect( this, SIGNAL( subProfileReceived( const Headers, const QByteArray ) ),
                 this, SLOT( onSubProfileReceived( const Headers, const QByteArray ) ) );
        connect( this, SIGNAL( bridgeContactRecieved( const Headers, const QByteArray ) ),
                 this, SLOT( onBridgeContactV2Received( const Headers, const QByteArray ) ) );

        subProfileSignals.insert( qpSubProfileSection::BRIDGE, "bridgeDataMapReceived" );
        subProfileSignals.insert( qpSubProfileSection::CONTACTS, "contactDataMapReceived" );
        subProfileSignals.insert( qpSubProfileSection::GROUPS, "groupDataMapReceived" );

        connect( this, SIGNAL( bridgeDataMapReceived( const QByteArray& ) ),
                 this, SLOT( onBridgeDataMapReceived( const QByteArray& ) ) );
        connect( this, SIGNAL( contactDataMapReceived( const QByteArray& ) ),
                 this, SLOT( onContactDataMapReceived( const QByteArray& ) ) );
        connect( this, SIGNAL( groupDataMapReceived( const QByteArray& ) ),
                 this, SLOT( onGroupDataMapReceived( const QByteArray& ) ) );
    }
}

void QPalringoConnection::setProxy( QNetworkProxy proxy )
{
    this->socket->setProxy( proxy );
}

int QPalringoConnection::connectClient( bool )
{
    this->socket = new QTcpSocket(this);
    socket->connectToHost( this->host, this->port );
    connect( socket, SIGNAL( connected() ), this, SLOT( connected() ) );
    connect( socket, SIGNAL( readyRead() ), this, SLOT( pollRead() ) );
    connect( socket, SIGNAL( error( QAbstractSocket::SocketError ) ), this, SLOT( socketError( QAbstractSocket::SocketError ) ) );

    return 1;
}

void QPalringoConnection::connected()
{
#if INFO
    qDebug( "Connected to server" );
#endif
    Headers headers;

    if (protocolVersion_ == 1)
    {
        headers.insert( qpHeaderAttribute::PROTOCOL_VERSION, "1.0" );
    }
    else if (protocolVersion_ == 2)
    {
        headers.insert( qpHeaderAttribute::PROTOCOL_VERSION, "2.0" );
        if( encryption_ )
        {
            headers.insert( qpHeaderAttribute::NAME, this->user.email );
        }
/*
        if( reconnect && RK_.size() )
        {
            headers.insert( qpHeaderAttribute::SUB_ID, this->user.userID );
        }

        if( !reconnect || !RK_.size() )
        {
            packetSeq_ = 0;
        }
*/
        if( compression_ )
        {
            headers.insert( qpHeaderAttribute::COMPRESSION, compression_ );
        }
    }

    headers.insert( qpHeaderAttribute::APP_TYPE, this->clientType );
    headers.insert( qpHeaderAttribute::OPERATOR, "PC_CLIENT" );
    sendCmd( qpCommand::LOGON, headers, "" );
}

void QPalringoConnection::socketError( QAbstractSocket::SocketError )
{
#if ERRORS
    qDebug( "%s", qPrintable( socket->errorString() ) );
#endif
    this->socket->disconnectFromHost();
    this->socket = NULL;
    emit disconnected();
}

void QPalringoConnection::pollRead()
{
#if qpDEBUG
    qDebug( "bytesAvailable = %lld", socket->bytesAvailable() );
#endif

    QByteArray tmp = socket->readAll();
    if( tmp.size() == 0 )
    {
        return;
    }
    this->inBuffer.append( tmp );

    int processed = parseCmd( this->inBuffer );

    while( !this->incomingCommands.isEmpty() )
    {
        IncomingCommand ic = this->incomingCommands.dequeue();

        if( ic.complete )
        {
            if( ic.headers.contains( qpHeaderAttribute::MESG_ID ) )
            {
                this->messageId = ic.headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
            }

            if( inSignals.contains( ic.command ) )
            {
#if SIGNALS
                qDebug( "emitting signal - %s", qPrintable( inSignals.value( ic.command ) ) );
#endif
                QMetaObject::invokeMethod( this, inSignals.value( ic.command ).toAscii().constData(), Qt::DirectConnection,
                                           Q_ARG( const Headers, ic.headers ),
                                           Q_ARG( const QByteArray, ic.body ) );
            }
        }
    }

    this->inBuffer = this->inBuffer.right( this->inBuffer.size() - processed );
}

bool QPalringoConnection::sendMessage( Target* target, Message message )
{
    // store this here for easy access
    QByteArray messageData = message.payload();

    QString command = qpCommand::MESG;
    Headers headers;

    // the data of the message
    qpMsgData data;
    data.targetId_ = target->getID();
    data.mesgTarget_ = ( target->getType() == Target::GROUP ) ? 1 : 0;
    data.mesgId_ = ++mesg_id_;
    data.contentType_ = message.type();

    if( message.bridgeID() != 0 )
    {
        data.bridgeId_ = message.bridgeID();
        command = qpCommand::BRIDGE_MESG;
    }

    // if the message is too big to go in 1 block
    if( message.payload().size() > MAX_PACKET_SIZE )
    {
        qint64 i = MAX_PACKET_SIZE;
        qint64 a = messageData.size() - ( messageData.size() % MAX_PACKET_SIZE );

        // set the data of the message
        headers = data.setData();

        // get the first MAX_PACKET_SIZE bytes
        QByteArray cdata = messageData.left( i );
        // send the first MAX_PACKET_SIZE bytes of the message
        sendCmd( qpCommand::MESG, headers, cdata );

        // set the correlarion ID for the rest of the messages
        data.correlationId_ = data.mesgId_;

        for( ; i < a; i += MAX_PACKET_SIZE )
        {
            // increment the message ID
            data.mesgId_ = ++mesg_id_;

            // re-set the headers for the message
            headers = data.setData();

            // put the data into the QByteArray
            cdata = messageData.mid( i, MAX_PACKET_SIZE );

            // send the next 512 bytes of the message
            sendCmd( command, headers, cdata );
        }

        // now got the last little bit to send
        data.mesgId_ = ++mesg_id_;
        data.last_ = true;
        headers = data.setData();

        cdata = messageData.right( messageData.size() - i );
        return sendCmd( command, headers, cdata );
    }
    else
    {
        // we just have a small message so we don't need to break it down into smaller chunks
        data.last_ = true;
        headers = data.setData();
        return sendCmd( command, headers, messageData );
    }
}

void QPalringoConnection::joinGroup( QString groupName )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::MESG_ID, QString::number( ++mesg_id_ ) );
    headers.insert( qpHeaderAttribute::NAME, groupName );

    QPalringoConnection::sendCmd( qpCommand::GROUP_SUBSCRIBE, headers, "" );
}

void QPalringoConnection::createGroup( QString groupName, QString groupDescription, QString groupPassword )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::MESG_ID, QString::number( ++mesg_id_ ) );
    headers.insert( qpHeaderAttribute::NAME, groupName );

    if( groupDescription.size() > 0 )
    {
        headers.insert( qpHeaderAttribute::DESC, groupDescription );
    }
    if( groupPassword.size() > 0 )
    {
        headers.insert( qpHeaderAttribute::CONTENT_LENGTH, QString::number( groupPassword.size() ) );
    }

    QPalringoConnection::sendCmd( qpCommand::GROUP_CREATE, headers, groupPassword.toUtf8() );
}

void QPalringoConnection::leaveGroup( quint64 groupID )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::MESG_ID, QString::number( ++mesg_id_ ) );
    headers.insert( qpHeaderAttribute::GROUP_ID, QString::number( groupID ) );

    QPalringoConnection::sendCmd( qpCommand::GROUP_UNSUB, headers, "" );
}

bool QPalringoConnection::updateContactDetail( QString detail, QString value )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::MESG_ID, ++mesg_id_ );
    headers.insert( detail, value );

    QPalringoConnection::sendCmd( qpCommand::CONTACT_DETAIL, headers );

    return true;
}

void QPalringoConnection::setBridgeStatus( quint32 bridgeId, qpOnlineStatus::OnlineStatus onlineStatus )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::MESG_ID, ++mesg_id_ );
    headers.insert( qpHeaderAttribute::BRIDGE_ID, bridgeId );
    headers.insert( qpHeaderAttribute::ONLINE_STATUS, onlineStatus );

    QPalringoConnection::sendCmd( qpCommand::BRIDGE_ON, headers );
}

void QPalringoConnection::getMesgHist( Target *target, QString timestampStr, qint32 count )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::MESG_ID, ++mesg_id_ );
    headers.insert( qpHeaderAttribute::COUNT, count );
    headers.insert( qpHeaderAttribute::SOURCE_ID, target->getID() );

    if( target->getType() == Target::CONTACT )
    {
        headers.insert( qpHeaderAttribute::FROM_PRIVATE, timestampStr );
    }
    else if( target->getType() == Target::GROUP )
    {
        headers.insert( qpHeaderAttribute::FROM_GROUP, timestampStr );
    }

    QPalringoConnection::sendCmd( qpCommand::MESG_HIST, headers, "" );
}

bool QPalringoConnection::sendCmd( QString command, Headers headers, QByteArray body)
{
    QByteArray outPacket;
    outPacket.append( command + "\n" );

    // Headers
    QHashIterator<QString, QVariant> headerIterator(headers);
    while( headerIterator.hasNext() )
    {
        headerIterator.next();

        outPacket.append( headerIterator.key() );
        outPacket.append( ": " );
        outPacket.append( headerIterator.value().toString() );
        outPacket.append( "\n" );
    }

    // Content-Length
    if (body.size() > 0 )
    {
        outPacket.append( "CONTENT-LENGTH: " );
        outPacket.append( QString::number( body.size() ) );
        outPacket.append( "\n" );
    }

    // End of headers
    outPacket.append("\n");
    outPacket.append( body );

    this->socket->write( outPacket );

    if( outSignals.contains( command ) )
    {
#if SIGNALS
        qDebug( "emitting signal - %s", qPrintable( outSignals.value( command ) ) );
#endif
        QMetaObject::invokeMethod( this, outSignals.value( command ).toAscii().constData(), Qt::DirectConnection,
                                   Q_ARG( Headers, headers ),
                                   Q_ARG( QByteArray, body ) );
    }

    return true;
}


User QPalringoConnection::getUser()
{
    return this->user;
}

Group* QPalringoConnection::getGroup( quint64 groupID )
{
    if( this->groups.contains( groupID ) )
    {
        return this->groups.value( groupID );
    }
    return NULL;
}

Contact* QPalringoConnection::getContact( quint64 contactID )
{
    Contact* c = NULL;
    this->contactLock.lockForRead();
    if( this->contacts.contains( contactID ) )
    {
        c = this->contacts.value( contactID );
    }
    this->contactLock.unlock();
    return c;
}

Bridge* QPalringoConnection::getBridge( quint32 bridgeID )
{
    if( this->bridges.contains( bridgeID ) )
    {
        return this->bridges.value( bridgeID );
    }
    return NULL;
}

BridgeContact* QPalringoConnection::getBridgeContact( quint32 bridgeID, quint64 contactID )
{
    Bridge* b = this->bridges.value( bridgeID );
    if( b )
    {
        if( b->getContacts().contains( contactID ) )
        {
            return b->getContacts().value( contactID );
        }
    }
    return NULL;
}

QHash<quint64, Contact*> QPalringoConnection::getAllContacts()
{
    return this->contacts;
}

QHash<quint64, Contact*> QPalringoConnection::getContactListContacts()
{
    QHash<quint64, Contact*> contacts;

    this->contactLock.lockForRead();
    foreach( Contact *contact, this->contacts )
    {
        if( contact->getIsContact() )
        {
#if qpDEBUG
            qDebug( "Inserting contact" );
#endif
            contacts.insert( contact->getID(), contact );
        }
        else
        {
#if qpDEBUG
            qDebug( "not a contact" );
#endif
        }
    }
    this->contactLock.unlock();

    return contacts;
}

QHash<quint64, Contact*> QPalringoConnection::getGroupContacts( quint64 groupID )
{
    QHash<quint64, Contact*> groupContacts;

    Group *group = this->getGroup( groupID );
    QHash<quint64, qint32> groupContactIDs = group->getContacts();
    this->contactLock.lockForRead();
    QHashIterator<quint64, qint32> i(groupContactIDs);
    while( i.hasNext() )
    {
        i.next();
        Contact* contact = this->contacts.value( i.key() );
        groupContacts.insert( contact->getID(), contact );
    }
    this->contactLock.unlock();

    return groupContacts;
}

int QPalringoConnection::parseCmd( const QByteArray& data )
{
#if PARSING
    qDebug( "processing - %d", data.size() );
#endif
    QByteArray endOfLine = "\r\n";
    QByteArray endOfPacket = "\r\n\r\n";
    QByteArray headerSpliter = ": ";

    int totalProcessed = 0;

    while( totalProcessed < data.size() )
    {
        int endOfPacketPos = data.indexOf( endOfPacket, totalProcessed );
        int contentLength;

#if PARSING
        qDebug( "\ttotalProcessed = %d", totalProcessed );
        qDebug( "\tendOfPacketPos = %d", endOfPacketPos );
#endif

        if( endOfPacketPos > -1 )
        {
            int contentLengthPos = data.indexOf( qpHeaderAttribute::CONTENT_LENGTH, totalProcessed );
#if PARSING
            qDebug( "\t\tgot a full packet" );
            qDebug( "\t\tcontentLengthPos = %d", contentLengthPos );
#endif

            if( ( contentLengthPos > -1 ) && ( contentLengthPos < endOfPacketPos ) )
            {
                int eol = data.indexOf( endOfLine, contentLengthPos );
#if PARSING
                qDebug( "\t\t\tWe have a content length in the message" );
                qDebug( "\t\t\teol - %d", eol );
#endif

                if( ( eol > -1 ) && ( eol < data.size() ) )
                {
                    int offset = contentLengthPos + qpHeaderAttribute::CONTENT_LENGTH.length() + headerSpliter.length();

                    QByteArray s = data.mid( offset, eol - offset );;
                    contentLength = s.toInt();
#if PARSING
                    qDebug( "\t\t\t\tContent Length = %d", contentLength );
#endif
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                contentLength = 0;
#if PARSING
                qDebug( "\t\t\tNo content length, assume that there's no content length = %d", contentLength );
#endif
            }
        }
        else
        {
#if PARSING
            qDebug( "Don't have the end of packet" );
#endif
            break;
        }

        if( ( contentLength + endOfPacketPos ) > data.size() )
        {
#if PARSING
            qDebug( "\t\tCONTENT LENGTH GOES BEYOND DATA SIZE." );
#endif
            break;
        }
        QString s = data.mid( totalProcessed, data.indexOf( endOfLine, totalProcessed ) - totalProcessed );
#if PARSING
        qDebug( "\t\tCommand - %s", qPrintable( s ) );
#endif
        Headers headers;

        int i = data.indexOf( endOfLine, totalProcessed ) + endOfLine.size();
        while( i < endOfPacketPos )
        {
            if( data.indexOf( endOfLine, i ) == i )
            {
                i += 2;
                break;
            }

            int x = data.indexOf( headerSpliter, i );
            int eol = data.indexOf( endOfLine, i );

            if( ( x == -1) || ( eol > endOfPacketPos ) )
            {
                break;
            }
            else
            {
                QString key = (data.mid( i, x - i )).toUpper();
                QString value = data.mid( x + headerSpliter.size(), eol - ( x + headerSpliter.size() ) );
#if PARSING
                qDebug( "\t\tinserting key: %s, value: %s", qPrintable( key ), qPrintable( value ) );
#endif
                headers.insert( key, value );

                i = eol + endOfLine.size();
            }
        }
        i += endOfLine.size();

        IncomingCommand ic;
        ic.command = data.mid( totalProcessed, data.indexOf( endOfLine, totalProcessed ) - totalProcessed );
        ic.headers = headers;
        ic.complete = true;

        if( contentLength > 0 )
        {
            ic.body = data.mid( i, contentLength );
        }

        this->incomingCommands.enqueue( ic );

        totalProcessed = i + contentLength;
    /**
        // Extract headers
        headers.clear();
        const char* header = crlf + 2;
        while (true)
        {
            if (strncmp(header, "\r\n", 2) == 0)
            {
                header += 2;
                break;
            }
            const char* const cs = strstr(header, ": ");
            const char* const crlf = strstr(header, "\r\n");
            if (cs == 0 || crlf == 0)
            {
                std::cerr << "Should never happen!" << std::endl;
                return false; // Should never happen!
            }
            headers[std::string (header, cs - header)] =
                    std::string (cs + 2, crlf - cs - 2);
            header = crlf + 2;
        }

        // And the rest is the body (if any)
        body = std::string(header, eom - header);
        sofar_ += eom - inBuf;

        return true;
    **/
    }
#if PARSING
    qDebug( "totalProcessed = %d", totalProcessed );
#endif
    return totalProcessed;
}
