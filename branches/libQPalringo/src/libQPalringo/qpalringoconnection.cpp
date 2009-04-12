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
                                         unsigned int port ) :
        PalringoConnection(host.toStdString(), port, login.toStdString(), password.toStdString(), "", true, 1, false, 0, clientType.toStdString() )
{
    this->user.email = login;
    this->user.password = password;
}

void QPalringoConnection::run()
{
    try
    {
        while( this->poll() > -1 )
        {
            msleep( 42 );
        }
    }
    catch (int error)
    {
        qDebug( "error %d", error );
    }
}

int QPalringoConnection::onMesgReceived(headers_t& headers,
                                         std::string& body,
                                         GenericData *data __attribute__ ((unused)) )
{
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
}

int QPalringoConnection::onLogonSuccessfulReceived( headers_t &headers, std::string &body, GenericData *data )
{
    PalringoConnection::onLogonSuccessfulReceived( headers, body, data );

    this->user.userID = userId_;
    this->user.nickname = QString::fromStdString( nickname_ );
    this->user.status = QString::fromStdString( status_ );
    this->user.lastOnline = QString::fromStdString( lastOnline_ );

    this->serverTimestamp = QString::fromStdString( serverTimestamp_ );

    emit( logonSuccessful( serverTimestamp ) );

    return 1;
}

int QPalringoConnection::onContactDetailReceived(headers_t& headers,
                                               std::string& body,
                                               GenericData *data __attribute__ ((unused)) )
{
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
}

int QPalringoConnection::onGroupDetailReceived(headers_t& headers,
                          std::string& body,
                          GenericData *data __attribute__ ((unused)) )
{
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
}

bool QPalringoConnection::sendMessage( Target* target, Message message )
{
    // store this here for easy access
    QByteArray messageData = message.payload();

    headers_t headers;

    // the data of the message
    MsgData data;
        data.targetId_ = target->getID();
        data.mesgTarget_ = ( target->getType() == Target::GROUP ) ? 1 : 0;;
        data.mesgId_ = ++mesg_id_;
        data.contentType_ = message.type().toStdString();

    // we need this here since MsgData::setData wants an std::string (even though it's not used)
    std::string s = "";

    // if the message is too big to go in 1 block
    if( message.payload().size() > MAX_PACKET_SIZE )
    {
        qint64 i = MAX_PACKET_SIZE;
        qint64 a = messageData.size() - ( messageData.size() % MAX_PACKET_SIZE );

        // set the data of the message
        data.setData( headers, s );

        // get the first MAX_PACKET_SIZE bytes
        QByteArray cdata = messageData.left( i );
        // send the first MAX_PACKET_SIZE bytes of the message
        sendCmd(pCommand::MESG, headers, cdata.data(), cdata.size() );

        // set the correlarion ID for the rest of the messages
        data.correlationId_ = data.mesgId_;

        for( i;
             i < a;
             i += MAX_PACKET_SIZE )
        {
            // increment the message ID
            data.mesgId_ = ++mesg_id_;

            // re-set the headers for the message
            data.setData( headers, s );

            // put the data into the QByteArray
            cdata = messageData.mid( i, MAX_PACKET_SIZE );

            // send the next 512 bytes of the message
            sendCmd( pCommand::MESG, headers, cdata.data(), cdata.size() );
        }

        // now got the last little bit to send
        data.mesgId_ = ++mesg_id_;
        data.last_ = true;
        data.setData( headers, s );

        cdata = messageData.right( messageData.size() - i );
        return sendCmd( pCommand::MESG, headers, cdata.data(), cdata.size() );
    }
    else
    {
        // we just have a small message so we don't need to break it down into smaller chunks
        data.last_ = true;
        data.setData( headers, s );
        return sendCmd( pCommand::MESG, headers, messageData.data(), messageData.size() );
    }
}

void QPalringoConnection::joinGroup( QString groupName )
{
    PalringoConnection::groupSubscribe( groupName.toStdString() );
}

void QPalringoConnection::createGroup( QString groupName, QString groupDescription, QString groupPassword )
{
    PalringoConnection::groupCreate( groupName.toStdString(), groupDescription.toStdString(), groupPassword.toStdString() );
}

void QPalringoConnection::leaveGroup( quint64 groupID )
{
    PalringoConnection::groupUnsubscribe( groupID );
}

int QPalringoConnection::onSubProfileReceived(headers_t& headers,
                                               std::string& body,
                                               GenericData *data )
{
    qDebug( "QPalringoConnection::onSubProfileReceived - not implemented" );
    return PalringoConnection::onSubProfileReceived( headers, body, data );
}

bool QPalringoConnection::updateContactDetail( QString detail, QString value )
{
    headers_t h;
    h[ "MESG-ID" ] = toString(++mesg_id_);
    h[ detail.toStdString() ] = value.toStdString();
    PalringoConnection::sendCmd( pCommand::CONTACT_DETAIL, h );

    return true;
}


void QPalringoConnection::getMesgHist( Target *target, QString timestampStr, qint32 count )
{
    headers_t headers;
    headers["MESG-ID"] = QString::number( ++mesg_id_ ).toStdString();
    headers["COUNT"] = QString::number( count ).toStdString();
    headers["SOURCE-ID"] = QString::number( target->getID() );

    if( target->getType() == Target::CONTACT )
    {
        headers["FROM-PRIVATE"] = timestampStr.toStdString();
    }
    else if( target->getType() == Target::GROUP )
    {
        headers["FROM-GROUP"] = timestampStr.toStdString();
    }

    PalringoConnection::sendCmd( pCommand::MESG_HIST, headers, "" );
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
