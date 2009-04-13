/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as         *
 *  published by the Free Software Foundation, either version 3 of the     *
 *  License, or (at your option) any later version.                        *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU Lesser General Public License for more details.                    *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with QPalringo. If not, see                              *
 *  <http://www.gnu.org/licenses/>                                         *
 *                                                                         *
 ***************************************************************************/
#include <QPixmapCache>
#include <QTextDocument>
#include <QMessageBox>
#include "tools.h"
#include "contactpropertieswindow.h"
Tools *tools_;

Tools::Tools( PalringoWindow *mainWindow )
{
    this->mainWindow = mainWindow;
    this->loggedIn = false;

    // used for tracking history requests
    this->gettingHistory = false;
    this->historyTarget = NULL;

    QPixmapCache::setCacheLimit( 1024 * 5 );

    //this->serverTimestamp = new QDateTime();
}

Tools::~Tools() { }

bool Tools::checkChatWindowOpen( Target *target )
{
    return ( this->openWindows.value( target ) != NULL );
}

void Tools::openChatWindow( Target *target )
{
    if ( this->checkChatWindowOpen( target ) )
    {
        ChatWindow *w = this->openWindows.value( target );
        w->raise();
        w->activateWindow();
    }
    else
    {
        ChatWindow *w = new ChatWindow( this->mainWindow, target );
        this->openWindows[ target ] = w;
        w->show();
    }
}

void Tools::removeChatWindow( Target *target )
{
    this->openWindows.remove( target );
}

void Tools::historyMessageReceived( Message message )
{
    if( this->gettingHistory )
    {
        ChatWindow *w = this->openWindows.value( this->historyTarget );
        w->appendMessage( message );

        this->gettingHistory = false;
        this->historyTarget = NULL;
    }
}

void Tools::messageReceived( Message message )
{
    this->playSound( ":/sounds/new-message.wav" );
    Target* t = NULL;
    if( message.groupID() == 0 )
    {
        t = this->connection->getContact( message.senderID() );
    }
    else
    {
        t = this->connection->getGroup( message.groupID() );
    }

    if( t != NULL )
    {
        t->addMessage( message );
        if( t->getPendingMessages().size() == 1 )
        {
            emit( newPendingMessage( t ) );
        }
    }
}

void Tools::openPalringoConnection( QString email, QString password )
{
    if( this->loggedIn == false )
    {
        QString client;
        #ifdef Q_WS_WIN
            client = "x86";
        #endif
        #ifdef Q_WS_MAC
            client = "Mac";
        #endif
        #ifdef Q_WS_X11
            client = "Linux";
        #endif
        this->connection = new QPalringoConnection( email, password, client );

        if( connection->connectClient() == 1 )
        {
            connect( connection, SIGNAL( logonSuccessful( QString ) ), this, SLOT( logonSuccessful( QString ) ) );
            connect( connection, SIGNAL( gotGroupDetails( Group* ) ), this, SIGNAL( newGroupAdded( Group* ) ) );
            connect( connection, SIGNAL( gotContactDetails( Contact* ) ), this, SLOT( addContact( Contact* ) ) );
            connect( connection, SIGNAL( messageReceived( Message ) ), this, SLOT( messageReceived( Message ) ) );
            connect( connection, SIGNAL( historyMessageReceived( Message ) ), this, SLOT( historyMessageReceived( Message ) ) );
            connect( connection, SIGNAL( finished() ), tools_, SLOT( disconnected() ) );

            connection->start();
        }
    }
}

void Tools::sendMessage( Target *target, Message message )
{
    this->connection->sendMessage( target, message );
}

void Tools::getHistoryMessage( Target *target, QString timestamp )
{
    if( !this->gettingHistory )
    {
        this->gettingHistory = true;
        this->historyTarget = target;

        this->connection->getMesgHist( target, timestamp, 1 );
    }
}

void Tools::addContact( Contact *contact )
{
    if( this->loggedIn )
    {
        if( contact->getIsContact() )
        {
            emit( userContactReceived( contact ) );
        }
        emit( contactDetailReceived( contact ) );
    }
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
    QString link = "<a href=\"%1%2\">%3</a>";
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
        //insertText = link.arg(protocol, QString(href).replace('&', "\x0b").replace('%', "\x03"), href) + append;
        insertText = link.arg(protocol, QString(href).replace('%', "\x03"), href) + append;
        filteredLine.replace(pos, urlLen, insertText);
        pos += insertText.length();
    }

    // Change & to &amp; to prevent html entities to do strange things to the text
    //filteredLine.replace('&', "&amp;");
    //filteredLine.replace("\x0b", "&");

    return filteredLine;
}

QString Tools::formatMessageText( QByteArray messagePayload )
{
    QString message = QString::fromUtf8( messagePayload );

    message = Qt::escape( message );
    if( message.startsWith( "/me ") )
    {
        message.remove(0,4); // remove "/me "
        message.prepend("<span style=\"color:red;\">* ").append(" *</span>"); //add a coloured span
    }

    message.replace(QRegExp("(\r\n)|(\n)|(\r)"), "<br />");
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

QDateTime Tools::getMessageTimestamp( Message message )
{
    QDateTime msgTimestamp = QDateTime::fromTime_t( message.seconds() );
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
    delete this->connection;
    this->loggedIn = false;

    QMessageBox msgBox;
    msgBox.setText( "You have been disconnected from Palringo." );
    msgBox.setInformativeText( "Do you wish to reconnect?" );
    msgBox.setIcon( QMessageBox::Information );
    msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton( QMessageBox::Yes );
    int ret = msgBox.exec();

    if( ret == QMessageBox::Yes )
    {
        this->openPalringoConnection();
    }
    else
    {
        emit( cleanUp() );
    }
}

QPixmap Tools::getPixmap( QString iconFilename )
{
    QPixmap p;
    if( !QPixmapCache::find( iconFilename, p ) )
    {
        p.load( iconFilename );
        QPixmapCache::insert( iconFilename, p );
    }
    return p;
}

void Tools::showContactProperties( Contact *contact )
{
    ContactPropertiesWindow *c = ContactPropertiesWindow::getInstance();
    c->update( contact );
    c->show();
}

void Tools::joinGroup( QString groupName )
{
    this->connection->joinGroup( groupName );
}

void Tools::createGroup( QString groupName )
{
    this->connection->createGroup( groupName );
}

void Tools::leaveGroup( quint64 groupID )
{
    this->connection->leaveGroup( groupID );
    emit( groupLeft( groupID ) );
}

bool Tools::updateContactDetail( QString detail, QString value )
{
    return this->connection->updateContactDetail( detail, value );
}

bool Tools::isLoggedIn()
{
    return this->loggedIn;
}

void Tools::playSound( QString fileName __attribute__ ((unused)) )
{
    /*
    Phonon::MediaObject *mediaObject = new Phonon::MediaObject( this );
    mediaObject->setCurrentSource( Phonon::MediaSource( fileName ) );
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput( Phonon::MusicCategory, this );
    Phonon::Path path = Phonon::createPath( mediaObject, audioOutput );
    mediaObject->play();
    */
}

User Tools::getUser()
{
    return this->connection->getUser();
}

Contact* Tools::getContact( quint64 contactID )
{
    return this->connection->getContact( contactID );
}

QHash<quint64, Contact*> Tools::getContactListContacts()
{
    return this->connection->getContactListContacts();
}

QHash<quint64, Contact*> Tools::getGroupContacts( quint64 groupID )
{
    return this->connection->getGroupContacts( groupID );
}