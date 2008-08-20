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

    //connect( this,      SIGNAL( logonSuccessful() ), tools_, SLOT( logonSuccessful() ) );
    connect( this,      SIGNAL( gotContacts() ),tools_, SLOT( updateContacts() ) );
    connect( this,      SIGNAL( gotGroupDetails( Group* ) ), tools_, SLOT  ( addGroup( Group* ) ) );
    connect( this,      SIGNAL( gotContactDetails( Contact* ) ), tools_, SLOT  ( addContact( Contact* ) ) );
    connect( this,      SIGNAL( messageReceived( QString, unsigned long long, unsigned long long, QString ) ),
             tools_, SLOT  ( messageReceived( QString, unsigned long long, unsigned long long, QString ) ) );
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
    if( PalringoConnection::onMesgReceived( headers, body, &msgData ) )
    {
        qDebug( "got a message" );
        if( msgData.contentType_ == "text/plain" )
        {
            unsigned long long senderID = msgData.sourceId_;
            unsigned long long groupID  = msgData.targetId_ || 0;
            QString ct = "text/plain";
            QString message = QString::fromStdString( body );

            qDebug( "Contact ID %llu said: `%s` in group: %llu", senderID, qPrintable( message ), groupID );

            emit( messageReceived( message, senderID, groupID, ct ) );
        }
    }
    return 0;
}

int QPalringoConnection::onLogonSuccessfulReceived( headers_t &headers, std::string &body, GenericData *data )
{
    PalringoConnection::onLogonSuccessfulReceived( headers, body, data );
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

        qDebug( "got a contact - %s - %d", qPrintable( QString::fromStdString( contactData.nickname_ ) ), contactData.onlineStatus_ );

        Contact *contact = new Contact;
        contact->setNickname( QString::fromStdString( contactData.nickname_ ) );
        contact->setStatusline( QString::fromStdString( contactData.status_ ) );
        contact->setOnlineStatus( contactData.onlineStatus_ );
        contact->setIsContact( contactData.isContact_ );
        contact->setDeviceType( contactData.deviceType_ );
        contact->setID( contactData.contactId_ );
        
        emit( gotContactDetails( contact ) );

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
        qDebug( "got some group data - %s", qPrintable( groupName ) );

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
