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

QPalringoConnection::QPalringoConnection(QString login,
                                         QString password,
                                         QString clientType,
                                         QString host,
                                         quint16 port ) :
        PalringoConnection(host.toStdString(), port, login.toStdString(), password.toStdString(), "", true, 1, false, 0, clientType.toStdString() )
{
    this->host = host;
    this->port = port;
    this->clientType = clientType;

    this->user.email = login;
    this->user.password = password;

    this->socket = new QTcpSocket(this);

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

    connect( this, SIGNAL( pingSent( Headers&, QByteArray&, qpGenericData* ) ),              this, SLOT( onPingSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( logonSent( Headers&, QByteArray&, qpGenericData* ) ),             this, SLOT( onLogonSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( byeSent( Headers&, QByteArray&, qpGenericData* ) ),               this, SLOT( onByeSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( authSent( Headers&, QByteArray&, qpGenericData* ) ),              this, SLOT( onAuthSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( contactUpdateSent( Headers&, QByteArray&, qpGenericData* ) ),     this, SLOT( onContactUpdateSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( contactAddRespSent( Headers&, QByteArray&, qpGenericData* ) ),    this, SLOT( onContactAddRespSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupSubscribeSent( Headers&, QByteArray&, qpGenericData* ) ),    this, SLOT( onGroupSubscribeSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupUnsubSent( Headers&, QByteArray&, qpGenericData* ) ),        this, SLOT( onGroupUnsubSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupCreateSent( Headers&, QByteArray&, qpGenericData* ) ),       this, SLOT( onGroupCreateSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupInviteSent( Headers&, QByteArray&, qpGenericData* ) ),       this, SLOT( onGroupInviteSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupAdminSent( Headers&, QByteArray&, qpGenericData* ) ),        this, SLOT( onGroupAdminSent ( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( mesgSent( Headers&, QByteArray&, qpGenericData* ) ),              this, SLOT( onMesgSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( mesgStoredSent( Headers&, QByteArray&, qpGenericData* ) ),        this, SLOT( onMesgStoredSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( mesgHistSent( Headers&, QByteArray&, qpGenericData* ) ),          this, SLOT( onMesgHistSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( regSent( Headers&, QByteArray&, qpGenericData* ) ),               this, SLOT( onRegSent( Headers&, QByteArray&, qpGenericData* ) ) );
}

void QPalringoConnection::initInSignals()
{
    inSignals.insert( qpCommand::AUTH, "authRecieved" );
/*
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
*/

    connect( this, SIGNAL( authRecieved( const Headers&, const QByteArray&, qpGenericData* ) ),              this, SLOT( onAuthRecieved( const Headers&, const QByteArray&, qpGenericData* ) ) );
/*
    connect( this, SIGNAL( logonSent( Headers&, QByteArray&, qpGenericData* ) ),             this, SLOT( onLogonSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( byeSent( Headers&, QByteArray&, qpGenericData* ) ),               this, SLOT( onByeSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( authSent( Headers&, QByteArray&, qpGenericData* ) ),              this, SLOT( onAuthSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( contactUpdateSent( Headers&, QByteArray&, qpGenericData* ) ),     this, SLOT( onContactUpdateSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( contactAddRespSent( Headers&, QByteArray&, qpGenericData* ) ),    this, SLOT( onContactAddRespSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupSubscribeSent( Headers&, QByteArray&, qpGenericData* ) ),    this, SLOT( onGroupSubscribeSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupUnsubSent( Headers&, QByteArray&, qpGenericData* ) ),        this, SLOT( onGroupUnsubSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupCreateSent( Headers&, QByteArray&, qpGenericData* ) ),       this, SLOT( onGroupCreateSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupInviteSent( Headers&, QByteArray&, qpGenericData* ) ),       this, SLOT( onGroupInviteSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( groupAdminSent( Headers&, QByteArray&, qpGenericData* ) ),        this, SLOT( onGroupAdminSent ( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( mesgSent( Headers&, QByteArray&, qpGenericData* ) ),              this, SLOT( onMesgSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( mesgStoredSent( Headers&, QByteArray&, qpGenericData* ) ),        this, SLOT( onMesgStoredSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( mesgHistSent( Headers&, QByteArray&, qpGenericData* ) ),          this, SLOT( onMesgHistSent( Headers&, QByteArray&, qpGenericData* ) ) );
    connect( this, SIGNAL( regSent( Headers&, QByteArray&, qpGenericData* ) ),               this, SLOT( onRegSent( Headers&, QByteArray&, qpGenericData* ) ) );
*/
}

int QPalringoConnection::connectClient( bool reconnect )
{
    if( ghosted_ )
    {
        ghosted_ = false;
    }

    socket->connectToHost( this->host, this->port );
    connect(socket, SIGNAL(readyRead()), this, SLOT(pollRead()));
    connect( socket, SIGNAL( error( QAbstractSocket::SocketError ) ), this, SLOT( socketError( QAbstractSocket::SocketError ) ) );

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

        if( reconnect && RK_.size() )
        {
            headers.insert( qpHeaderAttribute::SUB_ID, this->user.userID );
        }

        if( !reconnect || !RK_.size() )
        {
            packetSeq_ = 0;
        }

        if( compression_ )
        {
            headers.insert( qpHeaderAttribute::COMPRESSION, compression_ );
        }
    }

    headers.insert( qpHeaderAttribute::APP_TYPE, this->clientType );
    headers.insert( qpHeaderAttribute::OPERATOR, "PC_CLIENT" );
    sendCmd( qpCommand::LOGON, headers, "" );

    return 1;
}

void QPalringoConnection::socketError( QAbstractSocket::SocketError socketError )
{
    qDebug( "%s", qPrintable( socket->errorString() ) );
}

void QPalringoConnection::pollRead()
{
    qDebug( "something to read" );
    qDebug( "bytesAvailable = %lld", socket->bytesAvailable() );

    QByteArray tmp = socket->readAll();
    if( tmp.size() == 0 )
    {
        return;
    }
    this->inBuffer.append( tmp );

    IncomingCommand ic = parseCmd( this->inBuffer.constData() );

    if( ic.headers.contains( qpHeaderAttribute::MESG_ID ) )
    {
        this->messageId = ic.headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
    }
    
    if( inSignals.contains( ic.command ) )
    {
        qDebug( "emitting signal - %s", qPrintable( inSignals.value( ic.command ) ) );
        QMetaObject::invokeMethod( this, inSignals.value( ic.command ).toAscii().constData(), Qt::DirectConnection,
                                   Q_ARG( const Headers&, ic.headers ),
                                   Q_ARG( const QByteArray&, ic.body ),
                                   Q_ARG( qpGenericData*, NULL ) );
    }

    this->inBuffer.clear();
}

void QPalringoConnection::run()
{
    try
    {
        while( 1 ) //this->poll() > -1 )
        {
            //msleep( 42 );
            msleep( 3000 );
        }
    }
    catch (int error)
    {
        qDebug( "error %d", error );
    }
}

bool QPalringoConnection::sendMessage( Target* target, Message message )
{
    // store this here for easy access
    QByteArray messageData = message.payload();

    Headers headers;

    // the data of the message
    qpMsgData data;
        data.targetId_ = target->getID();
        data.mesgTarget_ = ( target->getType() == Target::GROUP ) ? 1 : 0;;
        data.mesgId_ = ++mesg_id_;
        data.contentType_ = message.type();

    // if the message is too big to go in 1 block
    if( message.payload().size() > MAX_PACKET_SIZE )
    {
        qint64 i = MAX_PACKET_SIZE;
        qint64 a = messageData.size() - ( messageData.size() % MAX_PACKET_SIZE );

        // set the data of the message
        headers = data.setData( "" );

        // get the first MAX_PACKET_SIZE bytes
        QByteArray cdata = messageData.left( i );
        // send the first MAX_PACKET_SIZE bytes of the message
        QPalringoConnection::sendCmd( qpCommand::MESG, headers, messageData );

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
            QPalringoConnection::sendCmd( qpCommand::MESG, headers, messageData );
        }

        // now got the last little bit to send
        data.mesgId_ = ++mesg_id_;
        data.last_ = true;
        headers = data.setData();

        cdata = messageData.right( messageData.size() - i );
        return QPalringoConnection::sendCmd( qpCommand::MESG, headers, messageData );
    }
    else
    {
        // we just have a small message so we don't need to break it down into smaller chunks
        data.last_ = true;
        headers = data.setData();
        return QPalringoConnection::sendCmd( qpCommand::MESG, headers, messageData );
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
/*
    outStream_.append( outPacket.toStdString() );
    outStream_.append( body.constData() );
    outMessageCount_++;
*/
    if( outSignals.contains( command ) )
    {
        qDebug( "emitting signal - %s", qPrintable( outSignals.value( command ) ) );
        QMetaObject::invokeMethod( this, outSignals.value( command ).toAscii().constData(), Qt::DirectConnection,
                                   Q_ARG( Headers, headers ),
                                   Q_ARG( QByteArray&, body ),
                                   Q_ARG( qpGenericData*, NULL ) );
    }
/*
    CmdCallbackFunctionsMap::iterator fit( outCallbackFunctions_.find( command.toStdString() ) );

    if( fit != outCallbackFunctions_.end() )
    {
        (this->*fit->second)(headers, const_cast<std::string&>(body), NULL);
    }

    else
    {
        processUnknownOutgoing(cmd, headers, const_cast<std::string&>(body));
    }
*/
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
            contacts.insert( contact->getID(), contact );
        }
    }
    this->contactLock.unlock();

    return contacts;
}

QHash<quint64, Contact*> QPalringoConnection::getGroupContacts( quint64 groupID )
{
    QHash<quint64, Contact*> groupContacts;

    Group *group = this->getGroup( groupID );
    QSet<quint64> groupContactIDs = group->getContacts();

    this->contactLock.lockForRead();
    foreach( quint64 contactID, groupContactIDs )
    {
        Contact* contact = this->contacts.value( contactID );
        groupContacts.insert( contactID, contact );
    }
    this->contactLock.unlock();

    return groupContacts;
}


/** incoming slots */
void QPalringoConnection::onAuthRecieved( const Headers& headers, const QByteArray& body, qpGenericData* data )
{
    qpAuthData authData;
    authData.getData(headers, body);

    qDebug( "%d", authData.wordSize_ );

    if( authData.wordSize_ )
    {
        // Save the word-size, we need it for later
        wordSize_ = authData.wordSize_;
    }

    connectionStatus_ = CONN_AUTHENTICATION;

    // Let's send our login and password
    Headers newHeaders;

    qpAuthData outAuthData;
    QByteArray newBody;
    if( authData.encryptionType_ > -1 )
    {
        outAuthData.onlineStatus_ = qpOnlineStatus::ONLINE;

        // No Encryption
        if( authData.encryptionType_ == 0 )
        {
            outAuthData.encryptionType_ = 0;
            outAuthData.name_ = this->user.email;
            newBody.append( this->user.password );
            if( encryption_ )
            {
                encryption_ = false;
            }
        }
//
//        // Salsa20/MD5
//        else if( authData.encryptionType_ == 1 )
//        {
//            return;
//            /*
//            std::string challenge(body.substr(0, 16));
//            DBGOUT << "Challenge:\n" << hexDump(challenge) << std::endl;
//            std::string IV(body.substr(16, 8));
//            DBGOUT << "IV:\n" << hexDump(IV) << std::endl;
//            std::string keyBundle(passwordMD5_ + IV);
//            std::string keyStr(crypto::md5(keyBundle));
//            DBGOUT << "passwordMD5_:\n" << hexDump(passwordMD5_) << std::endl;
//            DBGOUT << "passwordMD5_ + IV:\n" << hexDump(keyBundle) << std::endl;
//            DBGOUT << "keyStr:\n" << hexDump(keyStr) << std::endl;
//
//            crypto::SalsaCipher salsa(IV, keyStr);
//            uint32_t randomness[4];
//            randomness[0] = rand();
//            randomness[1] = rand();
//            randomness[2] = rand();
//            randomness[3] = rand();
//            std::string randStr(reinterpret_cast<const char*>(randomness), 16);
//            std::string beforeEncrypt(challenge + randStr);
//            salsa.encrypt(beforeEncrypt, newBody);
//
//            DBGOUT << "Plain Body:\n" << hexDump(beforeEncrypt) << std::endl;
//            DBGOUT << "Encrypted Body:\n" << hexDump(newBody) << std::endl;
//
//            std::string newKey(crypto::md5(passwordMD5_ + randStr));
//            DBGOUT << "newKey:\n" << hexDump(newKey) << std::endl;
//
//            delete salsa_;
//            salsa_ = new crypto::SalsaCipher(IV, newKey);
//            */
//        }
//
//        // Salsa20/oldpassword
//        else if( authData.encryptionType_ == 2 )
//        {
//            return;
//            /*
//            std::string challenge(body.substr(0, 16));
//            DBGOUT << "Challenge:\n" << hexDump(challenge) << std::endl;
//            std::string IV(body.substr(16, 8));
//            DBGOUT << "IV:\n" << hexDump(IV) << std::endl;
//            std::string oldPass(crypto::oldPassword(password_));
//            std::string keyBundle(oldPass + IV);
//            std::string keyStr(crypto::md5(keyBundle));
//            DBGOUT << "oldPass:\n" << hexDump(oldPass) << std::endl;
//            DBGOUT << "oldPass + IV:\n" << hexDump(keyBundle) << std::endl;
//            DBGOUT << "keyStr:\n" << hexDump(keyStr) << std::endl;
//
//            crypto::SalsaCipher salsa(IV, keyStr);
//            uint32_t randomness[4];
//            randomness[0] = rand();
//            randomness[1] = rand();
//            randomness[2] = rand();
//            randomness[3] = rand();
//            std::string randStr(reinterpret_cast<const char*>(randomness), 16);
//            std::string beforeEncrypt(challenge +
//                                      randStr +
//                                      password_ +
//                                      std::string(50 - password_.size(), '\0'));
//            salsa.encrypt(beforeEncrypt, newBody);
//            DBGOUT << "Plain Body:\n" << hexDump(beforeEncrypt) << std::endl;
//            DBGOUT << "Encrypted Body:\n" << hexDump(newBody) << std::endl;
//
//            std::string newKey(crypto::md5(passwordMD5_ + randStr));
//            DBGOUT << "newKey:\n" << hexDump(newKey) << std::endl;
//
//            delete salsa_;
//            salsa_ = new crypto::SalsaCipher(IV, newKey);
//            */
//        }
//
        newHeaders = outAuthData.setData();
    }
//
//    // Reconnection
//    else
//    {
//        /*
//    std::string reconnectChallenge(RK_);
//    reconnectChallenge.append(body);
//    newBody = crypto::md5(reconnectChallenge);
//    char tmp[64];
//    sprintf(tmp, "%lu", --packetSeq_);
//    headers["PS"] = tmp;
//    loggedOn_ = true;
//    receivedData_ = 0;
//    */
//        return;
//    }
//
//
    sendCmd( qpCommand::AUTH, newHeaders, newBody );
}

void QPalringoConnection::onMesgReceived( const Headers& headers, const QByteArray& body, qpGenericData* data )
{
    /*
    MsgData msgData;
    PalringoConnection::onMesgReceived( headers, body, &msgData );

    Message message;
    bool last = msgData.last_;
    quint64 correlationID = msgData.correlationId_;
    quint64 messageID = msgData.mesgId_;
    if( !last )
    {
        if( correlationID > 0 )
        {
            // don't really need to do anything
        }
        else if( !unfinishedMessages.contains( messageID ) )
        {
            QString timestamp = QString::fromStdString( msgData.timestamp_ );
            message.setType( QString::fromStdString( msgData.contentType_ ) );
            message.setSenderID( msgData.sourceId_ );
            message.setGroupID( msgData.targetId_ | 0 );
            //message.setSeconds( timestamp.left( timestamp.indexOf( "." ) ).toInt() + tools_->getTimestampDifference() );
            message.setSeconds( timestamp.left( timestamp.indexOf( "." ) ).toInt() );
            message.setUseconds( timestamp.right( timestamp.indexOf( "." ) ).toInt() );
            message.setHist( msgData.hist_ );
            unfinishedMessages.insert( messageID, message );
        }
        return 0;
    }
    else if( correlationID > 0 )
    {
        message = unfinishedMessages.value( correlationID );
        QByteArray tmp = QByteArray::fromRawData( body.data(), body.size() );
        message.setPayload( message.payload().append( tmp ) );
        unfinishedMessages.remove( correlationID );
    }
    else
    {
        QString timestamp = QString::fromStdString( msgData.timestamp_ );
        message.setType( QString::fromStdString( msgData.contentType_ ) );
        message.setSenderID( msgData.sourceId_ );
        message.setGroupID( msgData.targetId_ | 0 );
        //message.setSeconds( timestamp.left( timestamp.indexOf( "." ) ).toInt() + tools_->getTimestampDifference() );
        message.setSeconds( timestamp.left( timestamp.indexOf( "." ) ).toInt() );
        message.setUseconds( timestamp.right( timestamp.indexOf( "." ) ).toInt() );
        message.setHist( msgData.hist_ );
        QByteArray tmp = QByteArray::fromRawData( body.data(), body.size() );
        message.setPayload( message.payload().append( tmp ) );
    }

    if( message.hist() == true )
    {
        emit( historyMessageReceived( message ) );
    }
    else
    {
        emit( messageReceived( message ) );
    }
    return 1;
    */
}

void QPalringoConnection::onLogonSuccessfulReceived( const Headers& headers, const QByteArray& body, qpGenericData* data )
{
    /*
    PalringoConnection::onLogonSuccessfulReceived( headers, body, data );

    this->user.userID = userId_;
    this->user.nickname = QString::fromStdString( nickname_ );
    this->user.status = QString::fromStdString( status_ );
    this->user.lastOnline = QString::fromStdString( lastOnline_ );

    this->serverTimestamp = QString::fromStdString( serverTimestamp_ );

    emit( logonSuccessful( serverTimestamp ) );

    return 1;
    */
}

void QPalringoConnection::onContactDetailReceived( const Headers& headers, const QByteArray& body, qpGenericData* data )
{
    /*
    ContactData contactData;
    if( PalringoConnection::onContactDetailReceived( headers, body, &contactData ) )
    {
        if( this->contacts.contains( contactData.contactId_ ) )
        {
            Contact* contact = this->contacts.value( contactData.contactId_ );

            if( contactData.nickname_.size() )
            {
                contact->setNickname( QString::fromStdString( contactData.nickname_ ) );
            }
            if( contactData.status_.size() )
            {
                contact->setStatusline( QString::fromStdString( contactData.status_ ) );
            }
            if( contactData.onlineStatus_ > -1 )
            {
                contact->setOnlineStatus( contactData.onlineStatus_ );
            }
        }
        else
        {
            Contact *contact = new Contact;
            contact->setNickname( QString::fromStdString( contactData.nickname_ ) );
            contact->setStatusline( QString::fromStdString( contactData.status_ ) );
            contact->setOnlineStatus( contactData.onlineStatus_ );
            contact->setIsContact( contactData.isContact_ );
            contact->setDeviceType( contactData.deviceType_ );
            contact->setID( contactData.contactId_ );

            this->contactLock.lockForWrite();
            this->contacts.insert( contactData.contactId_, contact );
            this->contactLock.unlock();

            emit( gotContactDetails( contact ) );
        }
    }
    return 1;
    */
}

void QPalringoConnection::onGroupDetailReceived( const Headers& headers, const QByteArray& body, qpGenericData* data )
{
    /*
    GroupData groupData;
    if( PalringoConnection::onGroupDetailReceived( headers, body, &groupData ) )
    {
        QString groupName = QString::fromStdString( groupData.name_ );

        group_t &group_ = groups_[groupData.groupId_];
        std::set<uint64_t>::iterator it;

        QSet<quint64> group_contacts;

        for( it = group_.contacts_.begin(); it != group_.contacts_.end(); it++)
        {
            group_contacts.insert( *it );
        }

        Group *group = new Group;
        group->setID( groupData.groupId_ );
        group->setName( QString::fromStdString( group_.name_ ) );
        group->setDescription( QString::fromStdString( group_.desc_ ) );
        group->setContacts( group_contacts );

        this->groups.insert( group->getID(), group );

        emit( gotGroupDetails( group ) );
    }
    return 0;
    */
}

void QPalringoConnection::onSubProfileReceived( const Headers& headers, const QByteArray& body, qpGenericData* data )
{
    /*
    qDebug( "QPalringoConnection::onSubProfileReceived - not implemented" );
    return PalringoConnection::onSubProfileReceived( headers, body, data );
    */
}

IncomingCommand QPalringoConnection::parseCmd( const QByteArray& data )
{
    IncomingCommand ic;

    QByteArray endOfLine = "\r\n";
    QByteArray endOfPacket = "\r\n\r\n";
    QByteArray headerSpliter = ": ";

    int endOfPacketPos = data.indexOf( endOfPacket );
    int contentLength;

    if( endOfPacketPos > -1 )
    {
        qDebug( "got a full packet" );
        int contentLengthPos = data.indexOf( qpHeaderAttribute::CONTENT_LENGTH );
        qDebug( "contentLengthPos = %d", contentLengthPos );

        if( ( contentLengthPos > -1 ) && ( contentLengthPos < endOfPacketPos ) )
        {
            qDebug( "We have a content length in the message" );
            int eol = data.indexOf( endOfLine, contentLengthPos );
            qDebug( "eol - %d", eol );

            if( ( eol > -1 ) && ( eol <= data.size() ) )
            {
                QByteArray s = data.mid( contentLengthPos + qpHeaderAttribute::CONTENT_LENGTH.length() + 2, eol );
                contentLength = s.toInt();
                qDebug( "Content Length = %d", contentLength );
            }
            else
            {
                return ic;
            }
        }
        else
        {
            contentLength = endOfPacketPos + endOfPacket.size();
            qDebug( "No content length, assume that content length endOfPacketPos + endOfPacket.size() = %d", contentLength );
        }
    }
    else
    {
        /**
         * do we have the end of the header?
        // See if we have a full message in the buffer, either with a content
        // length found in the headers or without
        else
        {
            // if we get some user data, just skip over it
            // FIXME
            const char* const ud = strstr(inBuf, "USER_DATA");
            if( ud != 0 )
            {
                sofar_ += endBuf - inBuf;
                return true;
            }
            else
            {
                return false;
            }
        }
        **/
        return ic;
    }

    Headers headers;

    int i = data.indexOf( endOfLine ) + endOfLine.size();
    while( i < data.size() )
    {
        if( data.indexOf( endOfLine, i ) == i )
        {
            i += 2;
            break;
        }

        int x = data.indexOf( headerSpliter, i );
        int eol = data.indexOf( endOfLine, i );

        QString key = data.mid( i, x - i );
        QString value = data.mid( x + headerSpliter.size(), eol - ( x + headerSpliter.size() ) );

        qDebug( "inserting key: %s, value: %s", qPrintable( key ), qPrintable( value ) );
        headers.insert( key, value );

        i = eol + endOfLine.size();
    }

    ic.command = data.mid( 0, data.indexOf( endOfLine ) );
    ic.headers = headers;
    ic.body = data.mid( i, data.size() - i );

    return ic;
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
