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
#include "services/palringoservice.h"

Tools *tools_;
QPalringoConnection* Tools::connection = NULL;

Tools::Tools( QObject *parent )
    : QObject( parent )
{
    this->loggedIn = false;

    // used for tracking history requests
    this->gettingHistory = false;
    this->historyTarget = NULL;

    //this->serverTimestamp = new QDateTime();
}

Tools::~Tools() { }

void Tools::messageReceived( Message message )
{
    QSettings settings;

    this->playSound( ":/sounds/new-message.wav" );
    Target* t = NULL;
    if( message.groupID() != 0 )
    {
        t = this->connection->getGroup( message.groupID() );
    }
    else
    {
        t = message.sender();
    }

    if( t != NULL )
    {
        t->addMessage( message );
        emit newPendingMessage( t );
    }
}

void Tools::openPalringoConnection( QString email, QString password )
{
    QSettings settings;

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

        QString serverHost = settings.value( "server/host", "primary.palringo.com" ).toString();
        int serverPort = settings.value( "server/port", 443 ).toInt();
        int protocolVersion = settings.value( "server/protocolVersion", 1 ).toInt();

        if( this->connection == NULL )
        {
            this->connection = new QPalringoConnection( email, password, client, serverHost, serverPort, protocolVersion );

            if( settings.value( "networt/proxy/enabled" ).toBool() == true )
            {
                QNetworkProxy proxy;
                if( settings.value( "networt/proxy/proxytype" ).toInt() == 1 )
                {
                    proxy.setType(QNetworkProxy::Socks5Proxy);
                }
                else
                {
                    proxy.setType(QNetworkProxy::HttpProxy);
                }
                proxy.setHostName( settings.value( "networt/proxy/host" ).toString() );
                proxy.setPort( settings.value( "networt/proxy/port" ).toInt() );
                proxy.setUser( settings.value( "networt/proxy/username" ).toString() );
                proxy.setPassword( settings.value( "networt/proxy/password" ).toString() );
                QNetworkProxy::setApplicationProxy( proxy );
            }
        }

        if( connection->connectClient() == 1 )
        {
            connect( connection, SIGNAL( logonSuccessful( QString ) ), this, SLOT( logonSuccessful( QString ) ) );
            connect( connection, SIGNAL( gotGroupDetails( Group* ) ), this, SIGNAL( newGroupAdded( Group* ) ) );
            connect( connection, SIGNAL( gotContactDetails( Contact* ) ), this, SLOT( addContact( Contact* ) ) );
            connect( connection, SIGNAL( gotServiceDetails( Service* ) ), this, SIGNAL( gotServiceDetails( Service* ) ) );
            connect( connection, SIGNAL( gotBridgeContact( BridgeContact* ) ), this, SIGNAL( gotBridgeContact( BridgeContact* ) ) );
            connect( connection, SIGNAL( messageReceived( Message ) ), this, SLOT( messageReceived( Message ) ) );
            connect( connection, SIGNAL( historyMessageReceived( Message ) ), this, SIGNAL( historyMessageReceived( Message ) ) );
            connect( connection, SIGNAL( disconnected() ), this, SLOT( disconnected() ) );
        }
    }
}

void Tools::sendMessage( Target *target, Message message )
{
    Tools::connection->sendMessage( target, message );
}

void Tools::getHistoryMessage( Target *target, QString timestamp )
{
    if( !this->gettingHistory )
    {
        this->gettingHistory = true;
        this->historyTarget = target;

        this->connection->getMesgHist( target, timestamp, -1 );
    }
}

void Tools::addContact( Contact *contact )
{
    if( contact->getIsContact() )
    {
        emit( userContactReceived( contact ) );
    }
    emit( contactDetailReceived( contact ) );
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

void Tools::HeaderWrite(QByteArray* data, bool stereo, short bitsPerSample, /* 16 */
                        int numberOfSamples, int sampleRate /* 16,000 */ )
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
        emit cleanUp();
    }
}

/*
void Tools::showContactProperties( Contact *contact )
{
    ContactPropertiesWindow *c = ContactPropertiesWindow::getInstance();
    c->update( contact );
    c->show();
}
*/
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

void Tools::setBridgeStatus( quint32 bridgeId, qpOnlineStatus::OnlineStatus onlineStatus )
{
    this->connection->setBridgeStatus( bridgeId, onlineStatus );
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
    return Tools::connection->getUser();
}

Contact* Tools::getContact( quint64 contactID )
{
    return Tools::connection->getContact( contactID );
}

QHash<quint64, Contact*> Tools::getContactListContacts()
{
    return Tools::connection->getContactListContacts();
}

QHash<quint64, Contact*> Tools::getGroupContacts( quint64 groupID )
{
    return Tools::connection->getGroupContacts( groupID );
}

Group* Tools::getGroup( quint64 groupID )
{
    return Tools::connection->getGroup( groupID );
}

Bridge* Tools::getBridge( quint32 bridgeID )
{
    return Tools::connection->getBridge( bridgeID );
}

BridgeContact* Tools::getBridgeContact( quint32 bridgeID, quint64 contactID )
{
    return Tools::connection->getBridgeContact( bridgeID, contactID );
}

void Tools::addInitialPalringoService()
{
    PalringoService *s = new PalringoService;
    s->setType( qpBridgeType::PALRINGO );
    s->setNickname( "Palringo" );
    s->setStatus( "Offline" );
    s->setOnlineStatus( qpOnlineStatus::OFFLINE );

    emit gotServiceDetails( s );
}
