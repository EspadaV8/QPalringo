//
// C++ Implementation: qpalringoconnection
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
    qDebug( "creating a connection" );
    PalringoConnection::connectClient();

    connect( this,      SIGNAL( logonSuccessful() ),             tools_, SLOT( logonSuccessful() ) );
    connect( this,      SIGNAL( gotGroupDetails( Group* ) ),     tools_, SLOT( addGroup( Group* ) ) );
    connect( this,      SIGNAL( gotContactDetails( Contact* ) ), tools_, SLOT( addContact( Contact* ) ) );
    connect( this,      SIGNAL( messageReceived( Message* ) ),   tools_, SLOT( messageReceived( Message* ) ) );
}

int QPalringoConnection::poll()
{
    return PalringoConnection::poll();
}

int QPalringoConnection::onMesgReceived(headers_t& headers,
                                         std::string& body,
                                         GenericData *data)
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
        QString tmp = QString::fromStdString( body );
        message->payload.append( tmp );
    }
    
    emit( messageReceived( message ) );
    return 1;
}

int QPalringoConnection::onLogonSuccessfulReceived( headers_t &headers, std::string &body, GenericData *data )
{
    PalringoConnection::onLogonSuccessfulReceived( headers, body, data );
    
    unsigned long long userID = userId_;
    QString nickname = QString::fromStdString( nickname_ );
    QString status = QString::fromStdString( status_ );
    QString lastOnline = QString::fromStdString( lastOnline_ );
    
    tools_->setUser( userID , nickname, status, lastOnline );
    emit( logonSuccessful() );

    return 1;
}

int QPalringoConnection::onContactDetailReceived(headers_t& headers,
                                               std::string& body,
                                               GenericData *data)
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
        /*
        QMap<int, contact_t> m( this->contacts_ );
        qDebug( "number of contacts %d", m.count() );

        QMapIterator<int, contact_t> i(m);
        while (i.hasNext())
        {
            i.next();

            Contact *contact = new Contact;
            contact->setNickname( QString::fromStdString( i.value().nickname_ ) );
            contact->setStatusline( QString::fromStdString( i.value().status_ ) );
            contact->setOnlineStatus( i.value().onlineStatus_ );
            contact->setIsContact( i.value().isContact_ );
            contact->setDeviceType( i.value().deviceType_ );
            contact->setID( i.key() );
            tools_->addNewContact( contact );
        }
        */
        // emit( gotContact() );
    }
}

int QPalringoConnection::onGroupDetailReceived(headers_t& headers,
                          std::string& body,
                          GenericData *data)
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
    qDebug( "about to send message" );
    return PalringoConnection::sendToContact( msg.data(),
                                                  msg.size(),
                                                  receiverID,
                                                  contentType.toStdString());
}

bool QPalringoConnection::sendToGroup(QByteArray msg,
                                      QString contentType,
                                      unsigned long long groupID )
{
    qDebug( "about to send message" );
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
    qDebug( "about to send message" );
    int targetType = (isGroup) ? 1 : 0;
    return PalringoConnection::sendMessage( msg.data(),
                                            msg.size(),
                                            contentType.toStdString(),
                                            targetID,
                                            targetType );
}
