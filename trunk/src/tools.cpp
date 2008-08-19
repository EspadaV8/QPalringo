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

void Tools::updateContacts()
{
    this->mainWindow->updateContacts();
}

void Tools::sendMessage( Contact *contact, Message *message )
{
    qDebug( "sending message..." );
    this->connection->sendMessage( contact->getID(), message );
}

void Tools::addNewContact( Contact *contact )
{
    this->contacts.insert( contact->getID(),  contact );

    if( contact->getIsContact() )
    {
        emit( userContactReceived( contact ) );
    }
    emit( contactDetailReceived( contact ) );
}

QHash<unsigned long long, Contact*> Tools::getContacts()
{
    return this->contacts;
}

void Tools::addGroup( Group *group )
{
    this->groups.insert( group->getID(),  group );
    emit( newGroupAdded( group ) );
}
