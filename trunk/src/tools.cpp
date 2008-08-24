//
// C++ Implementation: tools
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tools.h"
Tools *tools_;

Tools::Tools( PalringoWindow *mainWindow )
{
    this->mainWindow = mainWindow;
    this->connection = NULL;
    this->loggedIn = false;
    this->user = new User;
}

Tools::~Tools() { }

bool Tools::checkChatWindowOpen( Target *target )
{
    return ( this->openWindows.value( target ) != NULL );
}

void Tools::openChatWindow( Target *target, bool isGroup )
{
    if ( this->checkChatWindowOpen( target ) )
    {
        ChatWindow *w = this->openWindows.value( target );
        w->raise();
        w->activateWindow();
    }
    else
    {
        ChatWindow *w = new ChatWindow( this->mainWindow, target, isGroup );
        this->openWindows[ target ] = w;
        w->show();
    }
}

void Tools::openChatWindow( Contact *contact )
{
    this->openChatWindow( contact, false );
}

void Tools::openChatWindow( Group *group )
{
    this->openChatWindow( group, true );
}

void Tools::removeChatWindow( Target *target )
{
    qDebug( "removing chat window" );
    this->openWindows.remove( target );
}

void Tools::messageReceived( QString message, unsigned long long senderID, unsigned long long group, QString contentType )
{
    Contact *contact = this->contacts.value( senderID );

    if( group == 0)
    {
        if( !this->checkChatWindowOpen( contact ) )
        {
            this->openChatWindow( contact );
        }
        ChatWindow *w = this->openWindows.value( contact );
        w->appendMessage( message, contact, contentType );
    }
    else
    {
        Group* group_ = this->groups.value( group );
        if( !this->checkChatWindowOpen( group_ ) )
        {
            this->openChatWindow( group_ );
        }
        ChatWindow *w = this->openWindows.value( group_ );
        w->appendMessage( message, contact, contentType );
    }
}

void Tools::messageReceived( Message* message )
{
    if( message->groupID == 0 )
    {
        Contact *contact = this->contacts.value( message->senderID );
        if( !this->checkChatWindowOpen( contact ) )
        {
            this->openChatWindow( contact );
        }
        ChatWindow *w = this->openWindows.value( contact );
        w->appendMessage( message );
    }
    else
    {
        Group* group = this->groups.value( message->groupID );
        if( !this->checkChatWindowOpen( group ) )
        {
            this->openChatWindow( group );
        }
        ChatWindow *w = this->openWindows.value( group );
        w->appendMessage( message );
    }
}

void Tools::openPalringoConnection( QString email, QString password )
{
    if( this->connection == NULL )
    {
        qDebug( "email: %s - password: %s", qPrintable( email ) , qPrintable( password ) );
        this->connection = new Connection( email, password );
        connection->start();
    }
}

void Tools::sendMessage( Target *target, bool isGroup, Message *message )
{
    qDebug( "sending message..." );
    this->connection->sendMessage( target->getID(), isGroup, message );
}

void Tools::addContact( Contact *contact )
{
    if( !this->contacts.contains( contact->getID() ) )
    {
        this->contacts.insert( contact->getID(),  contact );

        if( this->loggedIn )
        {
            if( contact->getIsContact() )
            {
                emit( userContactReceived( contact ) );
            }
            emit( contactDetailReceived( contact ) );
        }
    }
    else
    {
    }
}

QHash<unsigned long long, Contact*> Tools::getContacts()
{
    return this->contacts;
}

Contact* Tools::getContact( unsigned long long contactID )
{
    return this->contacts.value( contactID );
}

QHash<unsigned long long, Contact*> Tools::getContacts( unsigned long long groupID )
{
    QHash<unsigned long long, Contact*> groupContacts;
    if( groupID == 0 )
    {
        foreach( Contact *contact, this->contacts )
        {
            if( contact->getIsContact() )
            {
                groupContacts.insert( contact->getID(), contact );
            }
        }
    }
    else
    {
        Group *group = this->groups.value( groupID );
        QSet<unsigned long long> groupContactIDs = group->getContacts();
        foreach( unsigned long long contactID, groupContactIDs )
        {
            Contact* contact = this->contacts.value( contactID );
            groupContacts.insert( contactID, contact );
        }
    }
    return groupContacts;
}

void Tools::addGroup( Group *group )
{
    this->groups.insert( group->getID(),  group );
    emit( newGroupAdded( group ) );
}

void Tools::logonSuccessful()
{
    this->loggedIn = true;
    emit( connected() );
}

void Tools::setUser( unsigned long long userID, QString nickname, QString status, QString lastOnline )
{
    this->user->userID = userID;
    this->user->nickname = nickname;
    this->user->status = status;
    this->user->lastOnline = lastOnline;
}
