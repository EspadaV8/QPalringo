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
#include <QHash>
#include <QMap>
#include <QSet>
#include "qpalringoconnection.h"
#include "tools.h"

QPalringoConnection::QPalringoConnection(QString login,
                                         QString password,
                                         QString host,
                                         unsigned int port) :
        PalringoConnection(host.toStdString(), port, login.toStdString(), password.toStdString(), "", false, 1, true )
{
    PalringoConnection::connectClient();

    connect( this,      SIGNAL( logonSuccessful( QString ) ),    tools_, SLOT( logonSuccessful( QString ) ) );
    connect( this,      SIGNAL( gotGroupDetails( Group* ) ),     tools_, SLOT( addGroup( Group* ) ) );
    connect( this,      SIGNAL( gotContactDetails( Contact* ) ), tools_, SLOT( addContact( Contact* ) ) );
    connect( this,      SIGNAL( messageReceived( Message* ) ),   tools_, SLOT( messageReceived( Message* ) ) );
    connect( this,      SIGNAL( historyMessageReceived( Message* ) ),   tools_, SLOT( historyMessageReceived( Message* ) ) );
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

    Message* message;
    bool last = msgData.last_;
    unsigned long long correlationID = msgData.correlationId_;
    unsigned long long messageID = msgData.mesgId_;
    if( !last )
    {
        if( correlationID > 0 )
        {
            // don't really need to do anything
        }
        else if( !unfinishedMessages.contains( messageID ) )
        {
            message = new Message;
            message->type = QString::fromStdString( msgData.contentType_ );
            message->senderID = msgData.sourceId_;
            message->groupID  = msgData.targetId_ | 0;
            message->timestamp = tools_->convertTimestampToQDateTime( QString::fromStdString( msgData.timestamp_ ), true );
            message->hist = msgData.hist_;
            unfinishedMessages.insert( messageID, message );
        }
        return 0;
    }
    else if( correlationID > 0 )
    {
        message = unfinishedMessages.value( correlationID );
        QString tmp = QString::fromStdString( body );
        message->payload.append( tmp );
        unfinishedMessages.remove( correlationID );
    }
    else
    {
        message = new Message;
        message->type = QString::fromStdString( msgData.contentType_ );
        message->senderID = msgData.sourceId_;
        message->groupID  = msgData.targetId_ | 0;
        message->timestamp = tools_->convertTimestampToQDateTime( QString::fromStdString( msgData.timestamp_ ), true );
        message->hist = msgData.hist_;
        QString tmp = QString::fromStdString( body );
        message->payload.append( tmp );
    }

    if( message->hist == true )
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

    unsigned long long userID = userId_;
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

        QSet<unsigned long long> group_contacts;

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
                                        unsigned long long receiverID )
{
    return PalringoConnection::sendToContact( msg.data(),
                                                  msg.size(),
                                                  receiverID,
                                                  contentType.toStdString());
}

bool QPalringoConnection::sendToGroup(QByteArray msg,
                                      QString contentType,
                                      unsigned long long groupID )
{
    return PalringoConnection::sendToGroup( msg.data(),
                                            msg.size(),
                                            groupID,
                                            contentType.toStdString());
}

bool QPalringoConnection::sendMessage(QByteArray msg,
                                      QString contentType,
                                      unsigned long long targetID,
                                      bool isGroup )
{
    int targetType = (isGroup) ? 1 : 0;
    std::string cT = contentType.toStdString();
    return PalringoConnection::sendMessage( msg.data(),
                                            msg.size(),
                                            cT,
                                            targetID,
                                            targetType );
}

bool QPalringoConnection::getHistoryMessage( qint64 target, bool isGroup, qint32 timestamp )

{
    int targetType = (isGroup) ? 1 : 0;
    int count = -1;

    PalringoConnection::getMesgHist( count, timestamp, target, targetType );
    return true;
}