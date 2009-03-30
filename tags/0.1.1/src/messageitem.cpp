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
#include <QFile>
#include <QStringList>
#include <QBuffer>
#include "messageitem.h"
#include "tools.h"
#include "speexdecoder.h"

MessageItem::MessageItem( QWidget *parent, Message message ) :
        QWidget( parent )
{
    this->contact = tools_->getContact( message.senderID() );

    this->message = message;

    this->messageIcon = new QSvgWidget;
    this->messageIcon->setFixedSize( 24, 24 );
    this->messageText = new QLabel;
    this->messageText->setWordWrap( true );
    this->messageText->setTextFormat( Qt::RichText );
    this->messageText->setTextInteractionFlags( Qt::TextBrowserInteraction );
    this->messageText->setOpenExternalLinks( true );

    this->sender = new QLabel( this->contact->getNickname() );
    this->sender->setAlignment( Qt::AlignLeft );

    this->timestamp = new QLabel( this->message.timestamp().toString( "dd-MM-yy hh:mm:ss" ) );
    this->timestamp->setAlignment( Qt::AlignRight );

    QString messageTypeIcon;
    if( this->message.type() == "text/plain" )
    {
        QString formattedText = tools_->formatMessageText( this->message.payload() );
        this->messageText->setText( formattedText );
        messageTypeIcon = ":/svg/text.svg";
    }
    else if( this->message.type().startsWith( "image" ) )
    {
        QImage im;
        im.loadFromData( this->message.payload() );
        QImage scaled = im.scaled( 100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation );

        QPixmap pi = QPixmap::fromImage( scaled );
        this->messageText->setPixmap( pi );
        messageTypeIcon = ":/svg/image.svg";
    }
    else if( this->message.type().startsWith( "audio" ) )
    {
        QPixmap p( ":/misc/rightarrow.png" );
        this->messageText->setPixmap( p );
        messageTypeIcon = ":/svg/voice.svg";
    }

    this->messageIcon->load( messageTypeIcon );
    this->layout = new QHBoxLayout;

    this->leftside = new QVBoxLayout;
    this->rightside = new QVBoxLayout;
    this->headers = new QHBoxLayout;

    this->leftside->addWidget( this->messageIcon );
    this->leftside->addStretch( 1 );

    this->headers->addWidget( this->sender );
    this->headers->addWidget( this->timestamp );

    this->rightside->addLayout( this->headers );
    this->rightside->addWidget( this->messageText );
    this->rightside->addStretch( 1 );

    this->layout->addLayout( this->leftside );
    this->layout->addLayout( this->rightside, 1 );
    this->setLayout( layout );
    ReloadStyleSheet();
}


MessageItem::~MessageItem()
{
}


void MessageItem::mousePressEvent( QMouseEvent *event )
{
    this->toSelect = true;
    event->ignore();
}

void MessageItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    if( this->message.type().startsWith( "image" ) )
    {
        QString t = "Image from " + this->contact->getNickname();
        QPixmap p;
        p.loadFromData( this->message.payload() );

        QLabel *l = new QLabel( 0, Qt::Window );
        l->setPixmap( p );
        l->setWindowTitle( t );
        l->show();
    }
    else if( this->message.type().startsWith( "audio" ) )
    {
        this->handleAudioClip();
    }

    emit( doubleClick() );
    event->accept();
}

void MessageItem::handleAudioClip()
{
    /*
    QStringList l = Phonon::BackendCapabilities::availableMimeTypes();
    for( int i = 0; i < l.size(); ++i )
    {
        qDebug() << l.at(i);
    }

    QList<Phonon::AudioOutputDevice> audioOutputDevices = Phonon::BackendCapabilities::availableAudioOutputDevices();
    for( int i = 0; i < audioOutputDevices.size(); ++i )
    {
        qDebug() << audioOutputDevices.at(i).name();
    }
    */
    /*
    SpeexDecoder* sd = new SpeexDecoder( this );

    char* data = this->message->payload.data();
    int size = this->message->payload.size();
    QByteArray decodedData = sd->decode( data, size );

    qDebug() << "dataSize - " << this->message->payload.size() << " decodedData.size() - " << decodedData.size();

    QFile decodedFile( "decodedData.wav" );
    if( decodedFile.open( QIODevice::ReadWrite ) )
    {
        decodedFile.write( decodedData );
    }

    tools_->HeaderWrite( &decodedData, false, 16, 0, 16000 );

    QFile headerFile( "withHeaderData.wav" );
    if( headerFile.open( QIODevice::ReadWrite ) )
    {
        headerFile.write( decodedData );
    }

    qDebug( "Creating audio buffer" );
    QBuffer* audio = new QBuffer( &decodedData );//&this->message->payload );
    audio->open( QIODevice::ReadOnly );


    qDebug( "Creating MediaObject" );
    mediaObject = Phonon::createPlayer( Phonon::CommunicationCategory, Phonon::MediaSource( audio ) ) ;

    qDebug( "Playing" );
    mediaObject->play();
    */
    /*
    qDebug( "Creating MediaObject" );
    mediaObject = new Phonon::MediaObject(this);
    connect( mediaObject, SIGNAL(metaDataChanged()), this, SLOT(showMetaData()) );
    connect( mediaObject, SIGNAL(aboutToFinish()), this, SLOT(finished()) );

    qDebug( "Creating MediaSource" );
    Phonon::MediaSource *ms = new Phonon::MediaSource( &headerFile );// "/home/andrew/downloads/Music/ss.mp3" );//audio );

    qDebug( "Setting source" );
    mediaObject->setCurrentSource( *ms );

    qDebug( "Creating AudioOutput" );
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);

    qDebug( "Creating Path" );
    Phonon::Path path = Phonon::createPath(mediaObject, audioOutput);

    qDebug( "Playing" );
    mediaObject->play();
    */

    /*
    Phonon::MediaObject m;

    qDebug( "Setting source" );
    m.setCurrentSource( audio );

    qDebug( "Playing" );
    m.play();
    */
}

void MessageItem::showMetaData()
{
    /*
    QMultiMap<QString, QString> mm = mediaObject->metaData();
    qDebug() << "metaData size - " << mm.size();
    QMapIterator<QString, QString> i(mm);
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key() << ": " << i.value();
    }
    */
}

void MessageItem::finished()
{
//    qDebug() << "finished - " << mediaObject->errorString();
}

bool MessageItem::getToSelect()
{
    return this->toSelect;
}

void MessageItem::setSelected( bool b )
{
    this->toSelect = false;
    this->setProperty( "selected", b );
    ReloadStyleSheet();
}

void MessageItem::ReloadStyleSheet()
{
    QFile sheet ( ":/styles/MessageItem.css" );

    if ( ! sheet.open ( QIODevice::ReadOnly ) )
    {
        qDebug( "failed to read the stylesheet resource: %s", qPrintable( sheet.fileName() ) );
    }
    else
    {
        this->setStyleSheet ( sheet.readAll() );
    }
}

void MessageItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

Message MessageItem::getMessage()
{
    return this->message;
}