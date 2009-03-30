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
#include "tools.h"

QPalringoConnection::QPalringoConnection(QString login,
                                         QString password,
                                         QString host,
                                         unsigned int port,
                                         QString clientType) :
        PalringoConnection(host.toStdString(), port, login.toStdString(), password.toStdString(), "", true, 1, false, 0, clientType.toStdString() )
{
    PalringoConnection::connectClient();

    connect( this,      SIGNAL( logonSuccessful( QString ) ),    tools_, SLOT( logonSuccessful( QString ) ) );
    connect( this,      SIGNAL( gotGroupDetails( Group* ) ),     tools_, SLOT( addGroup( Group* ) ) );
    connect( this,      SIGNAL( gotContactDetails( Contact* ) ), tools_, SLOT( addContact( Contact* ) ) );
    connect( this,      SIGNAL( messageReceived( Message ) ),   tools_, SLOT( messageReceived( Message ) ) );
    connect( this,      SIGNAL( historyMessageReceived( Message ) ),   tools_, SLOT( historyMessageReceived( Message ) ) );
}

int QPalringoConnection::poll()
{
    return PalringoConnection::poll();
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
            message.setType( QString::fromStdString( msgData.contentType_ ) );
            message.setSenderID( msgData.sourceId_ );
            message.setGroupID( msgData.targetId_ | 0 );
            message.setTimestamp( tools_->convertTimestampToQDateTime( QString::fromStdString( msgData.timestamp_ ), true ) );
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
        message.setType( QString::fromStdString( msgData.contentType_ ) );
        message.setSenderID( msgData.sourceId_ );
        message.setGroupID( msgData.targetId_ | 0 );
        message.setTimestamp( tools_->convertTimestampToQDateTime( QString::fromStdString( msgData.timestamp_ ), true ) );
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

    quint64 userID = userId_;
    QString nickname = QString::fromStdString( nickname_ );
    QString status = QString::fromStdString( status_ );
    QString lastOnline = QString::fromStdString( lastOnline_ );
    QString serverTimestamp = QString::fromStdString( serverTimestamp_ );

    tools_->setUser( userID , nickname, status, lastOnline );
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
        if( tools_->getContact( contactData.contactId_ ) )
        {
            Contact* contact = tools_->getContact( contactData.contactId_ );

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

bool QPalringoConnection::sendToContact(QByteArray msg,
                                        QString contentType,
                                        quint64 receiverID )
{
    return PalringoConnection::sendToContact( msg.data(),
                                                  msg.size(),
                                                  receiverID,
                                                  contentType.toStdString());
}

bool QPalringoConnection::sendToGroup(QByteArray msg,
                                      QString contentType,
                                      quint64 groupID )
{
    return PalringoConnection::sendToGroup( msg.data(),
                                            msg.size(),
                                            groupID,
                                            contentType.toStdString());
}

bool QPalringoConnection::sendMessage(QByteArray msg,
                                      QString contentType,
                                      Target* target )
{
    int targetType = ( target->getType() == Target::GROUP ) ? 1 : 0;
    std::string cT = contentType.toStdString();
    return PalringoConnection::sendMessage( msg.data(),
                                            msg.size(),
                                            cT,
                                            target->getID(),
                                            targetType );
}

bool QPalringoConnection::getHistoryMessage( Target* target, qint32 timestamp )
{
    int targetType = ( target->getType() == Target::GROUP ) ? 1 : 0;
    int count = -1;

    PalringoConnection::getMesgHist( count, timestamp, target->getID(), targetType );
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

