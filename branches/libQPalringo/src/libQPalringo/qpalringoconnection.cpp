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

            this->contacts.insert( contactData.contactId_, contact );

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

        emit( gotGroupDetails( group ) );
    }
    return 0;
}

bool QPalringoConnection::sendMessage(Target *target, Message message )
{
    int targetType = ( target->getType() == Target::GROUP ) ? 1 : 0;
    std::string cT = message.type().toStdString();

    return PalringoConnection::sendMessage( message.payload().data(),
                                            message.payload().size(),
                                            cT,
                                            target->getID(),
                                            targetType );
}

bool QPalringoConnection::getHistoryMessage( Target* target, QString timestamp )
{
    int targetType = ( target->getType() == Target::GROUP ) ? 1 : 0;
    int count = -1;

    PalringoConnection::getMesgHist( count, timestamp.toStdString(), target->getID(), targetType );
    return true;
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

User QPalringoConnection::getUser()
{
    return this->user;
}
