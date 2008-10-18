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

    // used for tracking history requests
    this->gettingHistory = false;
    this->historyTarget = NULL;
    this->historyTargetIsGroup = false;

    //this->serverTimestamp = new QDateTime();
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

void Tools::historyMessageReceived( Message* message )
{
    qDebug() << "got a history message";

    if( this->gettingHistory )
    {
        ChatWindow *w = this->openWindows.value( this->historyTarget );
        w->appendMessage( message );

        this->gettingHistory = false;
        this->historyTarget = NULL;
        this->historyTargetIsGroup = false;


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

void Tools::getHistoryMessage( Target *target, bool isGroup, QDateTime timestamp )
{
    qDebug( "getting history..." );

    if( !this->gettingHistory )
    {
        this->gettingHistory = true;
        this->historyTarget = target;
        this->historyTargetIsGroup = isGroup;

        this->connection->getHistoryMessage( target->getID(), isGroup, timestamp.toTime_t() );
    }
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

void Tools::logonSuccessful( QString timestamp )
{
    this->calcServerTimestampDifference( timestamp );
    this->loggedIn = true;
    emit( connected() );
}

quint32 Tools::getTimestampDifference()
{
    return this->timestampDifference;
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

void Tools::HeaderWrite(QByteArray* data,
                        bool        stereo,
                        short       bitsPerSample, // 16
                        int         numberOfSamples,
                        int         sampleRate // 16,000
                       )
{
    short numberOfChannels = (short)(stereo ? 2 : 1);
    int dataSize = ( numberOfSamples * bitsPerSample * numberOfChannels / 8 );
    int chunkSize = 36 + dataSize;
    int chunk1Size = 16;
    int byteRate = ( sampleRate * numberOfChannels * bitsPerSample / 8 );
    short blockAlign = ( numberOfChannels * bitsPerSample / 8 );

    if( numberOfSamples == 0 )
    {
        numberOfSamples = data->size() / 2; // ( (16000 * 2) / 8 );
    }

    qDebug( "size - %d : numberOfChannels - %d : bps - %d : numberOfSamples - %d : sampleRate - %d : chunkSize - %d", data->size(), numberOfChannels, bitsPerSample, numberOfSamples, sampleRate, chunkSize );

    union _WAV_UN
    {
        struct _WAVE_HEADER
        {
            int riffHeader;
            int chunkSize;
            int format;

            int subchunk1;
            int subchunk1size;
            short audioformat;
            short numChannels;
            int sampleRate;
            int byteRate;
            short blockAlign;
            short bps;

            int dataHeader;
            int dataSize;
        } wave_header;// WAVE_HEADER, *PWAVE_HEADER;
        char buffer[sizeof(_WAVE_HEADER)];
    };

    _WAV_UN waveHeader;
    _WAV_UN::_WAVE_HEADER* pHeader = &waveHeader.wave_header;
    pHeader->riffHeader     = 0x46464952;
    pHeader->chunkSize      = chunkSize;
    pHeader->format         = 0x45564157;

    pHeader->subchunk1      = 0x20746d66;
    pHeader->subchunk1size  = chunk1Size;
    pHeader->audioformat    = 1;
    pHeader->numChannels    = numberOfChannels;
    pHeader->sampleRate     = sampleRate;
    pHeader->byteRate       = byteRate;
    pHeader->blockAlign     = blockAlign;
    pHeader->bps            = bitsPerSample;

    pHeader->dataHeader     = 0x61746164;
    pHeader->dataSize       = dataSize;

    QByteArray t;

    for(unsigned int index=0;index<sizeof(_WAV_UN);index++)
        t.append(waveHeader.buffer[index]);

    data->prepend( t );
}


QDateTime Tools::convertTimestampToQDateTime( QString timestamp, bool receivedMessage )
{
    qint32 timestampSecs = timestamp.left( timestamp.indexOf( "." ) ).toInt();
    
    if ( receivedMessage )
    {
        timestampSecs += this->getTimestampDifference();
    }
    
    QDateTime msgTimestamp = QDateTime::fromTime_t( timestampSecs );

    return msgTimestamp;
}

void Tools::calcServerTimestampDifference( QString timestamp )
{
    quint32 timestampSecs  = timestamp.left( timestamp.indexOf( "." ) ).toInt();
    quint32 __attribute__ ((unused)) timestampUSecs = timestamp.right( timestamp.indexOf( "." ) ).toInt();
    
    QDateTime qdt = QDateTime::currentDateTime();
    quint32 currentSecs = qdt.toTime_t();
    
    this->timestampDifference = currentSecs - timestampSecs;
}

void Tools::disconnected()
{
    qDebug( "Tools::disconnected() - Not implemented" );
}
