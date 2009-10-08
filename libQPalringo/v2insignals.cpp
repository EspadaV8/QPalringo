#include "qpalringoconnection.h"

void QPalringoConnection::onSubProfileReceived( const Headers headers, const QByteArray body )
{
    if( body.size() )
    {
        if( this->compression_ && headers.contains( qpHeaderAttribute::COMPRESSION ) )
        {
            // TODO
            // body.replace( qUncompress( body.data() ) );
        }

        if( headers.contains( qpHeaderAttribute::RK ) )
        {
            if( this->encryption_ )
            {
                // TODO
            }
            else
            {
                this->RK_ = headers.attribute<QString>( qpHeaderAttribute::RK );
            }
        }

        qpLogonData logonData;
        logonData.getData( headers, body );

        QMapIterator<QString, QString> i( subProfileSignals );
        while( i.hasNext() )
        {
            i.next();
            if( logonData.dataMap_->contains( i.key().toAscii() ) )
            {
#if SIGNALS
                qDebug( "emitting signal - %s", qPrintable( i.value() ) );
#endif
                QMetaObject::invokeMethod( this, i.value().toAscii(), Qt::DirectConnection,
                                           Q_ARG( const QByteArray, logonData.dataMap_->value( i.key() ) ) );
            }
        }

        if( logonData.dataMap_->contains( qpHeaderAttribute::NICKNAME ) )
        {
            this->user.nickname = logonData.dataMap_->value( qpHeaderAttribute::NICKNAME );
        }
        if( logonData.dataMap_->contains( qpHeaderAttribute::STATUS ) )
        {
            this->user.status = logonData.dataMap_->value( qpHeaderAttribute::STATUS );
        }
        if( logonData.dataMap_->contains( qpHeaderAttribute::SUB_ID ) )
        {
            this->user.userID = (logonData.dataMap_->value( qpHeaderAttribute::SUB_ID )).toInt();
        }
        if( headers.contains( qpHeaderAttribute::LAST_ONLINE ) )
        {
            this->user.lastOnline =  headers.attribute<QString>( qpHeaderAttribute::LAST_ONLINE );
        }
        if( headers.contains( qpHeaderAttribute::TIMESTAMP ) )
        {
            emit logonSuccessful( headers.attribute<QString>( qpHeaderAttribute::TIMESTAMP ) );
        }
    }
}

void QPalringoConnection::onContactDataMapReceived( const QByteArray& data )
{
    qpDataMap contactsDataMap( data );

    QMapIterator<QString, QByteArray> contactsIterator( contactsDataMap );
    while( contactsIterator.hasNext() )
    {
        contactsIterator.next();
        qpDataMap contactDataMap( contactsIterator.value() );

        if( this->contacts.contains( contactsIterator.key().toInt() ) )
        {
            Contact* contact = this->contacts.value( contactsIterator.key().toInt() );

            if( contactDataMap.contains( qpHeaderAttribute::NICKNAME ) )
                contact->setNickname( contactDataMap.value( qpHeaderAttribute::NICKNAME) );

            if( contactDataMap.contains( qpHeaderAttribute::STATUS ) )
                contact->setStatusline( contactDataMap.value( qpHeaderAttribute::STATUS ) );

            if( contactDataMap.contains( qpHeaderAttribute::ONLINE_STATUS ) )
                contact->setOnlineStatus( contactDataMap.value( qpHeaderAttribute::ONLINE_STATUS ).toInt() );
        }
        else
        {
#if qpDEBUG
            qDebug( "New contact recieved" );
#endif
            Contact *contact = new Contact;
            contact->setNickname( contactDataMap.value( qpHeaderAttribute::NICKNAME ) );
            contact->setStatusline( contactDataMap.value( qpHeaderAttribute::STATUS ) );
            contact->setOnlineStatus( contactDataMap.value( qpHeaderAttribute::ONLINE_STATUS ).toInt() );
            contact->setIsContact( contactDataMap.contains( qpHeaderAttribute::CONTACT ) );
            contact->setDeviceType( contactDataMap.value( qpHeaderAttribute::DEVICE_TYPE ).toInt() );
            contact->setID( contactsIterator.key().toInt() );

            this->contactLock.lockForWrite();
            this->contacts.insert( contactsIterator.key().toInt(), contact );
            this->contactLock.unlock();
#if SIGNALS
            qDebug( "emitting gotContactDetails( Contact* )" );
#endif
            emit gotContactDetails( contact );
        }
    }
}

void QPalringoConnection::onGroupDataMapReceived( const QByteArray& data )
{
    qpDataMap groupsDataMap( data );

    QMapIterator<QString, QByteArray> groupsIterator( groupsDataMap );
    while( groupsIterator.hasNext() )
    {
        groupsIterator.next();
        qpDataMap groupDataMap( groupsIterator.value() );

        QHash<quint64, qint32> groupContacts;
        QMapIterator<QString, QByteArray> groupsContactIterator( groupDataMap );
        while( groupsContactIterator.hasNext() )
        {
            groupsContactIterator.next();
            qpDataMap groupContactDataMap( groupsContactIterator.value() );

            bool ok;
            int contactId = groupsContactIterator.key().toInt( &ok );
            if( ok )
            {
                qint32 capabilities = groupContactDataMap.value( qpHeaderAttribute::CAPABILITIES, 0 ).toInt();
                groupContacts.insert( contactId, capabilities );
            }
        }

        Group *group = new Group;
        group->setID( groupsIterator.key().toInt() );
        group->setName( groupDataMap.value( qpHeaderAttribute::NAME ) );
        group->setDescription( groupDataMap.value( qpHeaderAttribute::DESC ) );
        group->setContacts( groupContacts );

        this->groups.insert( group->getID(), group );

#if SIGNALS
        qDebug( "emitting gotContactDetails( Contact* )" );
#endif
        emit gotGroupDetails( group );
    }
}

void QPalringoConnection::onBridgeDataMapReceived( const QByteArray& data )
{
    qpDataMap bridgesDataMap( data );

    QMapIterator<QString, QByteArray> bridgesIterator( bridgesDataMap );
    while( bridgesIterator.hasNext() )
    {
        bridgesIterator.next();
        qpDataMap bridgeDataMap( bridgesIterator.value() );

        int bridgeId = bridgesIterator.key().toInt();

        Bridge *bridge = new Bridge;
        bridge->setNickname( bridgeDataMap.value( qpHeaderAttribute::NICKNAME ) );
        bridge->setUsername( bridgeDataMap.value( qpHeaderAttribute::USERNAME ) );
        bridge->setType( (qpBridgeType::Type)((bridgeDataMap.value( qpHeaderAttribute::BRIDGE_TYPE )).toInt()) );
        bridge->setId( bridgeId );
        bridge->setOnlineStatus( qpOnlineStatus::OFFLINE );

        this->bridges.insert( bridgeId, bridge );

        BridgeContact *contact = new BridgeContact;
        contact->setID( 0 );
        contact->setBridgeId( bridgeId );
        contact->setNickname( bridgeDataMap.value( qpHeaderAttribute::NICKNAME ) );
        contact->setName( bridgeDataMap.value( qpHeaderAttribute::USERNAME ) );
        contact->setOnlineStatus( qpOnlineStatus::ONLINE );

        bridge->addContact( contact );

#if SIGNALS
        qDebug( "emitting onBridgeReceived( Bridge* )" );
#endif
        emit gotServiceDetails( bridge );
    }
}

void QPalringoConnection::onBridgeContactV2Received( const Headers /* headers */, const QByteArray body )
{
    qpDataMap bridgeContacts( body );

    QMapIterator<QString, QByteArray> bridgeContactsIterator( bridgeContacts );
    while( bridgeContactsIterator.hasNext() )
    {
        // bridgeContactsIterator.key() is the bridge ID
        // bridgeContactsIterator.value() has the details of the bridge
        bridgeContactsIterator.next();

        int bridgeId = bridgeContactsIterator.key().toInt();
        Bridge* b = this->bridges.value( bridgeId );
        qpDataMap bridgeDetails( bridgeContactsIterator.value() );

        // if the datamap has some contacts we need to process those
        if( bridgeDetails.contains( qpSubProfileSection::CONTACTS ) )
        {
            qpDataMap contactsDataMap( bridgeDetails.value( qpSubProfileSection::CONTACTS ) );
            QMapIterator<QString, QByteArray> contactsIterator( contactsDataMap );
            while( contactsIterator.hasNext() )
            {
                contactsIterator.next();
                bool ok;
                int contactId = contactsIterator.key().toInt( &ok );
                qpDataMap contactDataMap( contactsIterator.value() );

                if( ok )
                {
                    if( b->getContacts().contains( contactId ) )
                    {
                        BridgeContact* contact = b->getContacts().value( contactId );

                        if( contactDataMap.contains( qpHeaderAttribute::NAME ) )
                            contact->setName( contactDataMap.value( qpHeaderAttribute::NAME ) );

                        if( contactDataMap.contains( qpHeaderAttribute::NICKNAME ) )
                            contact->setNickname( contactDataMap.value( qpHeaderAttribute::NICKNAME ) );

                        if( contactDataMap.contains( qpHeaderAttribute::USERNAME ) )
                            contact->setUsername( contactDataMap.value( qpHeaderAttribute::USERNAME ) );

                        if( contactDataMap.contains( qpHeaderAttribute::STATUS ) )
                            contact->setStatusline( contactDataMap.value( qpHeaderAttribute::STATUS ) );

                        if( contactDataMap.contains( qpHeaderAttribute::CURRENT_MEDIA ) )
                            contact->setCurrentMedia( contactDataMap.value( qpHeaderAttribute::CURRENT_MEDIA ) );

                        if( contactDataMap.contains( qpHeaderAttribute::ONLINE_STATUS ) )
                            contact->setOnlineStatus( (qpOnlineStatus::OnlineStatus)(contactDataMap.value( qpHeaderAttribute::ONLINE_STATUS ).toInt()) );
                    }
                    else
                    {
                        BridgeContact* contact = new BridgeContact;
                        contact->setID( contactId );
                        contact->setBridgeId( bridgeId );
                        contact->setName( contactDataMap.value( qpHeaderAttribute::NAME ) );
                        contact->setNickname( contactDataMap.value( qpHeaderAttribute::NICKNAME ) );
                        contact->setUsername( contactDataMap.value( qpHeaderAttribute::USERNAME ) );
                        contact->setStatusline( contactDataMap.value( qpHeaderAttribute::STATUS ) );
                        contact->setCurrentMedia( contactDataMap.value( qpHeaderAttribute::CURRENT_MEDIA ) );
                        contact->setOnlineStatus( (qpOnlineStatus::OnlineStatus)(contactDataMap.value( qpHeaderAttribute::ONLINE_STATUS ).toInt()) );

                        b->addContact( contact );
#if SIGNALS
                        qDebug( "emitting gotBridgeContact( BridgeContact* )" );
#endif
                        emit gotBridgeContact( contact );
                    }
                }
            }
        }
    }
}
