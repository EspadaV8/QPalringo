#include "qpalringoconnection.h"

void QPalringoConnection::onAuthRecieved( const Headers& headers, const QByteArray& body )
{
    qpAuthData authData;
    authData.getData(headers, body);

    if( authData.wordSize_ )
    {
        // Save the word-size, we need it for later
        wordSize_ = authData.wordSize_;
    }

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
//            if( encryption_ )
//            {
//                encryption_ = false;
//            }
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

void QPalringoConnection::onMesgReceived( const Headers& headers, const QByteArray& body )
{
    qpMsgData msgData;
    msgData.getData( headers, body );

    Message message;
    if( msgData.correlationId_ > 0 )
    {
        message = unfinishedMessages.value( msgData.correlationId_ );
        message.setPayload( message.payload().append( body ) );
        unfinishedMessages.insert( msgData.correlationId_, message );
    }
    else
    {
        message.setType( msgData.contentType_ );
        message.setSenderID( msgData.sourceId_ );
        message.setGroupID( msgData.targetId_ | 0 );
        //message.setSeconds( timestamp.left( timestamp.indexOf( "." ) ).toInt() + tools_->getTimestampDifference() );
        message.setSeconds( msgData.timestamp_.left( msgData.timestamp_.indexOf( "." ) ).toInt() );
        message.setUseconds( msgData.timestamp_.right( msgData.timestamp_.indexOf( "." ) ).toInt() );
        message.setBridgeID( msgData.bridgeId_ );
        message.setHist( msgData.hist_ );
        message.setName( msgData.name_ );
        message.setPayload( body );
        if( ( !msgData.last_ ) && ( !unfinishedMessages.contains( msgData.mesgId_ ) ) )
        {
            unfinishedMessages.insert( msgData.mesgId_, message );
        }
    }

    if( !msgData.last_ )
    {
#if SIGNALS
        qDebug( "not the last part of the message" );
#endif
        return;
    }
    else if( message.hist() == true )
    {
#if SIGNALS
        qDebug( "emitting historyMessageReceived( Message )" );
#endif
        emit historyMessageReceived( message );
    }
    else
    {
#if SIGNALS
        qDebug( "emitting messageReceived( Message )" );
#endif
        emit messageReceived( message );
    }
}

void QPalringoConnection::onLogonSuccessfulReceived( const Headers& headers, const QByteArray& body )
{
    qpLogonData logonData;
    logonData.getData( headers, body );

    this->user.userID = logonData.subId_;
    this->user.nickname = logonData.nickname_;
    this->user.status = logonData.status_;
    this->user.lastOnline = logonData.lastOnline_;

    this->serverTimestamp = logonData.timestamp_;

#if SIGNALS
    qDebug( "emitting logonSuccessful( QString )" );
#endif
    emit logonSuccessful( serverTimestamp );
}

void QPalringoConnection::onContactDetailReceived( const Headers& headers, const QByteArray& body )
{
    qpContactData contactData;
    contactData.getData( headers, body );

    if( this->contacts.contains( contactData.contactId_ ) )
    {
#if qpDEBUG
        qDebug( "We have this contact already" );
#endif
        Contact* contact = this->contacts.value( contactData.contactId_ );

        if( contactData.nickname_.size() )
        {
            contact->setNickname( contactData.nickname_ );
        }
        if( contactData.status_.size() )
        {
            contact->setStatusline( contactData.status_ );
        }
        if( contactData.onlineStatus_ > -1 )
        {
            contact->setOnlineStatus( contactData.onlineStatus_ );
        }
    }
    else
    {
#if qpDEBUG
        qDebug( "New contact recieved" );
#endif
        Contact *contact = new Contact;
        contact->setNickname( contactData.nickname_ );
        contact->setStatusline( contactData.status_ );
        contact->setOnlineStatus( contactData.onlineStatus_ );
        contact->setIsContact( contactData.isContact_ );
        contact->setDeviceType( contactData.deviceType_ );
        contact->setID( contactData.contactId_ );

        this->contactLock.lockForWrite();
        this->contacts.insert( contactData.contactId_, contact );
        this->contactLock.unlock();
#if SIGNALS
        qDebug( "emitting gotContactDetails( Contact* )" );
#endif
        emit gotContactDetails( contact );
    }
}

void QPalringoConnection::onGroupDetailReceived( const Headers& headers, const QByteArray& body )
{
    qpGroupData groupData;
    groupData.getData( headers, body );

    QSet<quint64> groupContacts;

    for( qint32 i = 0; i < ( body.size() / 8 ); ++i )
    {
        QByteArray t1 = body.mid( i * 8, 8 );

        /**
         * Thanks to Sotek and thiago_home from #qt for the following code
         */
        quint64 temp = 0;
        for( int j = 0; j < 8; j++ )
        {
            temp <<= 8;
            char c = t1.at( j );
            temp |= ( c & 0xff ) ;
        }
        groupContacts.insert( temp );
    }

    Group *group = new Group;
    group->setID( groupData.groupId_ );
    group->setName( groupData.name_ );
    group->setDescription( groupData.desc_ );
    group->setContacts( groupContacts );

    this->groups.insert( group->getID(), group );

#if SIGNALS
    qDebug( "emitting gotContactDetails( Contact* )" );
#endif
    emit gotGroupDetails( group );
}

void QPalringoConnection::onPingReceived( const Headers&, const QByteArray& )
{
    Headers h;
    if( protocolVersion_ == 2 )
    {
        h.insert( qpHeaderAttribute::PS, packetSeq_ );
    }

    sendCmd( qpCommand::PING, h, "");
}

void QPalringoConnection::onResponseReceived( const Headers& headers, const QByteArray& body )
{
    qpResponseData responseData;
    responseData.getData(headers, body);

#if INFO
    qDebug( "Response Message:\n\tWhat: %d\n\tMesg-Id: %llu\n\tType: %d",
            responseData.what_, responseData.mesgId_, responseData.type_ );
    if(!responseData.type_)
    {
        qDebug( "\tError Code: %d", responseData.errorCode_ );
    }
    else
    {
        qDebug( "\tError Message: %s", qPrintable( responseData.errorMessage_ ) );
    }
#endif
}

void QPalringoConnection::onBridgeReceived( const Headers& headers, const QByteArray& body )
{
    qpBridgeData bridgeData;
    bridgeData.getData(headers, body);

    Bridge *bridge = new Bridge;
    bridge->setNickname( bridgeData.nickname_ );
    bridge->setUsername( bridgeData.username_ );
    bridge->setType( bridgeData.type_ );
    bridge->setId( bridgeData.bridgeId_ );
    bridge->setOnlineStatus( qpOnlineStatus::OFFLINE );

    this->bridges.insert( bridgeData.bridgeId_, bridge );

    BridgeContact *contact = new BridgeContact;
    contact->setID( 0 );
    contact->setBridgeId( bridgeData.bridgeId_ );
    contact->setName( bridgeData.username_ );
    contact->setNickname( bridgeData.nickname_ );
    contact->setOnlineStatus( qpOnlineStatus::ONLINE );

    bridge->addContact( contact );

#if SIGNALS
    qDebug( "emitting onBridgeReceived( Bridge* )" );
#endif
    emit gotBridgeDetails( bridge );
}

void QPalringoConnection::onBridgeContactReceived( const Headers& headers, const QByteArray& body )
{
    qpBridgeContactData bridgeContactData;
    bridgeContactData.getData(headers, body);

    Bridge* b = this->bridges.value( bridgeContactData.bridgeId_ );

    if( b->getContacts().contains( bridgeContactData.contactId_ ) )
    {
        BridgeContact* contact = b->getContacts().value( bridgeContactData.contactId_ );

        if( bridgeContactData.name_.size() )
        {
            contact->setName( bridgeContactData.name_ );
        }
        if( bridgeContactData.nickname_.size() )
        {
            contact->setNickname( bridgeContactData.nickname_ );
        }
        if( bridgeContactData.status_.size() )
        {
            contact->setStatusline( bridgeContactData.status_ );
        }
        if( bridgeContactData.currentMedia_.size() )
        {
            contact->setCurrentMedia( bridgeContactData.currentMedia_ );
        }
        if( bridgeContactData.onlineStatus_ > -1 )
        {
            contact->setOnlineStatus( bridgeContactData.onlineStatus_ );
        }
    }
    else
    {
        BridgeContact* contact = new BridgeContact;
        contact->setID( bridgeContactData.contactId_ );
        contact->setBridgeId( bridgeContactData.bridgeId_ );
        contact->setName( bridgeContactData.name_ );
        contact->setNickname( bridgeContactData.nickname_ );
        contact->setStatusline( bridgeContactData.status_ );
        contact->setCurrentMedia( bridgeContactData.currentMedia_ );
        contact->setOnlineStatus( bridgeContactData.onlineStatus_ );

        b->addContact( contact );
#if SIGNALS
        qDebug( "emitting gotBridgeContact( BridgeContact* )" );
#endif
        emit gotBridgeContact( contact );
    }
}

void QPalringoConnection::onBridgeOnReceived( const Headers& headers, const QByteArray& )
{
    qpBridgeOnData bridgeOnData;
    bridgeOnData.getData( headers );

    if( this->bridges.contains( bridgeOnData.bridgeId_ ) )
    {
        this->bridges.value( bridgeOnData.bridgeId_ )->setOnlineStatus( qpOnlineStatus::ONLINE );
    }
}
