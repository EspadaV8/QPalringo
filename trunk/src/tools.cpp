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
    this->contactLock.lockForWrite();
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
    this->contactLock.unlock();
}

QHash<unsigned long long, Contact*> Tools::getContacts()
{
    return this->contacts;
}

Contact* Tools::getContact( unsigned long long contactID )
{
    Contact* c = NULL;
    this->contactLock.lockForRead();
    if( this->contacts.contains( contactID ) )
    {
        c = this->contacts.value( contactID );
    }
    this->contactLock.unlock();
    return c;
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

/*
 Taken from Konversation
 Copyright (C) 2004 Peter Simonsson <psn@linux.se>
 Copyright (C) 2006-2008 Eike Hein <hein@kde.org>

 Modified by Andrew Smith for Qt4
 Copyright (C) 2008 Andrew Smith <andrew@palringo.com>
*/
QString Tools::tagURLs( QString text )
{
    QRegExp urlPattern("((www\\.(?!\\.)|((f|sf|ht)tp(|s))://)(\\.?[\\d\\w/,\\':~\\?=;#@\\-\\+\\%\\*\\{\\}\\!\\(\\)]|&)+)|([-.\\d\\w]+@[-.\\d\\w]{2,}\\.[\\w]{2,})", Qt::CaseInsensitive, QRegExp::RegExp);
    QString link = "<u><a href=\"%1%2\">%3</a></u>";
    QString filteredLine = text;
    QString insertText;
    QString href;
    QString protocol;
    int pos = 0;
    int urlLen = 0;

    while((pos = urlPattern.indexIn(filteredLine, pos)) >= 0)
    {
        QString append;

        // check if the matched text is already replaced as a channel
        if ( filteredLine.lastIndexOf( "<a", pos ) > filteredLine.lastIndexOf( "</a>", pos ) )
        {
            ++pos;
            continue;
        }

        protocol = "";
        urlLen = urlPattern.matchedLength();
        href = filteredLine.mid( pos, urlLen );

        // Don't consider trailing comma part of link.
        if (href.right(1) == ",")
        {
            href.truncate(href.length()-1);
            append = ',';
        }

        // Don't consider trailing semicolon part of link.
        if (href.right(1) == ";")
        {
            href.truncate(href.length()-1);
            append = ';';
        }

        // Don't consider trailing closing parenthesis part of link when
        // there's an opening parenthesis preceding the beginning of the
        // URL or there is no opening parenthesis in the URL at all.
        if (href.right(1) == ")" && (filteredLine.mid(pos-1,1) == "(" || !href.contains("(")))
        {
            href.truncate(href.length()-1);
            append.prepend(")");
        }

        // Qt doesn't support (?<=pattern) so we do it here
        if((pos > 0) && filteredLine[pos-1].isLetterOrNumber())
        {
            pos++;
            continue;
        }

        if (urlPattern.cap(1).startsWith("www.", Qt::CaseInsensitive))
        {
            protocol = "http://";
        }
        else if (urlPattern.cap(1).isEmpty())
        {
            protocol = "mailto:";
        }

        // Use \x0b as a placeholder for & so we can readd them after changing all & in the normal text to &amp;
        // HACK Replace % with \x03 in the url to keep Qt from doing stupid things
        insertText = link.arg(protocol, QString(href).replace('&', "\x0b").replace('%', "\x03"), href) + append;
        filteredLine.replace(pos, urlLen, insertText);
        pos += insertText.length();
    }

    // Change & to &amp; to prevent html entities to do strange things to the text
    filteredLine.replace('&', "&amp;");
    filteredLine.replace("\x0b", "&");

    return filteredLine;
}

QString Tools::formatMessageText( QByteArray messagePayload )
{
    QString message = QString::fromUtf8( messagePayload );

    message = Qt::convertFromPlainText( message );
    message = this->tagURLs( message );

    return message;
}
