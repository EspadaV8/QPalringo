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
#include <QFileDialog>
#include <QBuffer>
#include <QtNetwork>
#include "chatwindow.h"
#include "targets/bridgecontact.h"
#include "tools.h"
#include "qptools.h"

ChatWindow::ChatWindow ( QWidget *parent, Target *target )
    : QWidget ( parent )
{
    this->setWindowFlags ( Qt::Window );
    this->setAcceptDrops(true);
    this->target = target;

    this->setWindowTitle( this->target->getTitle() );
    QString icon = QPTools::getTargetIcon( this->target );
    this->setWindowIcon( QPTools::getPixmap( icon ) );
    this->setAttribute ( Qt::WA_DeleteOnClose, true );

    // Create all the layouts
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins( 0, 0, 0, 0 );
    hbox->setSpacing( 0 );

    QVBoxLayout *vb = new QVBoxLayout;
    vb->setContentsMargins( 0, 0, 0, 0 );
    vb->setSpacing( 0 );

    // set up all the widgets
    this->historyButton = new QPushButton ( "History", this );
    connect( this->historyButton, SIGNAL( clicked() ), this, SLOT( askForHistory() ) );
    this->messageList = new MessageList( this );
    this->multiLineInput = new ChatTextEdit( this );
    this->voiceButton = new QPushButton ( "", this );
    this->imageButton = new QPushButton ( "", this );

    // connect to the return key, so we can send a message
    connect( this->multiLineInput, SIGNAL( returnPressed() ), this, SLOT(checkMessageInput()) );

    // add the icons to the buttons
    this->voiceButton->setIcon( QPTools::getPixmap( ":/svg/voice.svg" ) );
    this->voiceButton->setToolTip( "Press and hold to record voice message" );
    this->voiceButton->setEnabled( false );
    this->imageButton->setIcon( QPTools::getPixmap( ":/svg/image.svg" ) );
    this->imageButton->setToolTip( "Browse to send an image message" );
    connect( this->imageButton, SIGNAL( clicked() ), this, SLOT( loadImageFile() ) );

    // add the items to the bottom layout
    hbox->addWidget( this->multiLineInput );
    vb->addWidget( this->voiceButton );
    vb->addWidget( this->imageButton );
    hbox->addLayout( vb );

    QWidget *w = new QWidget( this );
    w->setLayout( hbox );

    QSplitter *splitter = new QSplitter( Qt::Vertical, this );
    splitter->setChildrenCollapsible( false );
    splitter->addWidget( this->messageList );
    splitter->addWidget( w );
    splitter->setStretchFactor( 0, 1 );

    // add everything to the window
    layout->addWidget ( this->historyButton );
    layout->addWidget( splitter );

    this->setLayout ( layout );
    this->resize( 492, 323 );

    if( this->target->getPendingMessages().size() > 0 )
    {
        this->getMessages();
    }

    connect( this->target, SIGNAL( pendingMessage() ), this, SLOT( getMessages() ) );
    connect( &this->networkAccessManager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( handleNetworkData( QNetworkReply* ) ) );

    this->multiLineInput->setFocus();
}

ChatWindow::~ChatWindow()
{
    emit chatWindowClosed( this->target );
    delete this->historyButton;
    delete this->voiceButton;
    delete this->imageButton;
    delete this->messageList;
    delete this->multiLineInput;
}

void ChatWindow::loadImageFile()
{
    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.gif *.svg *.bmp *.tiff)"));

    if( fileName != "" )
    {
        QImage image( fileName );
        if ( !image.isNull() )
        {
            this->sendImageMessage( image );
        }
    }
}

void ChatWindow::sendImageMessage( QImage image )
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg");

    Message m;
    m.setType( "image/jpeg" );
    m.setPayload( buffer.data() );

    this->sendMessage( m );
}

void ChatWindow::sendTextMessage( QString message )
{
    Message m;
    m.setType( "text/plain" );
    m.setPayload( message.toUtf8() );

    this->sendMessage( m );
}

void ChatWindow::sendMessage( Message message )
{
    message.setSeconds( QDateTime::currentDateTime().toTime_t() );
    message.setUseconds( 0 );
    if( this->target->getType() == Target::BRIDGECONTACT )
    {
        BridgeContact *b = qobject_cast<BridgeContact*>( this->target );
        message.setBridgeID( b->getBridgeId() );
    }
    else
    {
        message.setBridgeID( 0 );
    }
    message.setHist( false );

    emit sendMessage( this, this->target, message );
}

void ChatWindow::checkMessageInput()
{
    QString message = this->multiLineInput->toPlainText();
    if( !message.isEmpty() )
    {
        this->multiLineInput->setPlainText("");
        this->sendTextMessage( message.toUtf8() );
    }
}

void ChatWindow::appendMessage( Message message )
{
    this->messageList->addMessage( message );
}

void ChatWindow::askForHistory()
{
    Message m = this->messageList->getMessageAt( 0 );
    QString timestamp ;

    if( m.seconds() != 0 )
    {
        timestamp = QString::number( m.seconds() ) + "." + QString::number( m.useconds() );
    }
    else
    {
        timestamp = QString::number( QDateTime::currentDateTime().toTime_t() ) + ".0";
    }
    //tools_->getHistoryMessage( this->target, timestamp );
}

void ChatWindow::getMessages()
{
    foreach( Message message, this->target->getPendingMessages() )
    {
        this->appendMessage( message );
    }
    this->target->clearPending();
}

void ChatWindow::keyPressEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Escape )
    {
        this->close();
    }
    else
    {
        QWidget::keyPressEvent( event );
    }
}

void ChatWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void ChatWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if( urls.count() )
    {
        QUrl url = urls[0];
        if( event->mimeData()->hasImage() )
        {
            QImage image = qvariant_cast<QImage>( event->mimeData()->imageData() );
            this->sendImageMessage( image );
        }
        else
        {
            networkAccessManager.get( QNetworkRequest( url ) );
        }
        event->acceptProposedAction();
    }
    else if( event->mimeData()->hasText() )
    {
        this->sendTextMessage( event->mimeData()->text() );
        event->acceptProposedAction();
    }
}

void ChatWindow::handleNetworkData( QNetworkReply *networkReply )
{
    if( networkReply->error() )
    {
        qDebug() << "Can't download" << networkReply->url().toString() << ":" << networkReply->errorString();
    }
    else
    {
        QByteArray ba = networkReply->readAll();
        QImage image;
        if( image.loadFromData( ba, 0 ) )
        {
            this->sendImageMessage( image );
        }
        else
        {
            this->sendTextMessage( networkReply->url().toString() );
        }
    }
    networkReply->deleteLater();
}
