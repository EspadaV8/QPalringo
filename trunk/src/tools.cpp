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
}

Tools::~Tools() { }

bool Tools::checkChatWindowOpen( Contact *contact )
{
    return ( this->openWindows.value( contact ) != NULL );
}


void Tools::openChatWindow( Contact *contact )
{
    if ( this->checkChatWindowOpen( contact ) )
    {
        ChatWindow *w = this->openWindows.value( contact );
        w->raise();
        w->activateWindow();
    }
    else
    {
        ChatWindow *w = new ChatWindow( this->mainWindow, contact );
        this->openWindows[ contact ] = w;
        w->show();
    }
}

void Tools::removeChatWindow( Contact *contact )
{
    qDebug( "removing chat window" );
    this->openWindows.remove( contact );
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

void Tools::sendMessage( Contact *contact, Message *message )
{
    qDebug( "sending message..." );
    this->connection->sendMessage( contact->getID(), message );
}

void Tools::sendMessage( Group *group, Message *message )
{
    qDebug( "sending message..." );
    this->connection->sendToGroup( group->getID(), message );
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
