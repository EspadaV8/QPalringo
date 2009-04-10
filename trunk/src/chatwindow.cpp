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
#include "chatwindow.h"
#include "tools.h"

ChatWindow::ChatWindow ( PalringoWindow *parent, Target *target )
    : QWidget ( parent )
{
    this->setWindowFlags ( Qt::Window );
    this->parent = parent;
    this->target = target;

    this->setWindowTitle( this->target->getTitle() );
    this->setWindowIcon ( tools_->getPixmap( this->target->getIcon() ) );
    this->setAttribute ( Qt::WA_DeleteOnClose, true );

    // Create all the layouts
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    QVBoxLayout *vb = new QVBoxLayout;

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
    this->voiceButton->setIcon( tools_->getPixmap( ":/svg/voice.svg" ) );
    this->voiceButton->setToolTip( "Press and hold to record voice message" );
    this->voiceButton->setEnabled( false );
    this->imageButton->setIcon( tools_->getPixmap( ":/svg/image.svg" ) );
    this->imageButton->setToolTip( "Browse to send an image message" );
    connect( this->imageButton, SIGNAL( clicked() ), this, SLOT( loadImageFile() ) );

    // add the items to the bottom layout
    hbox->addWidget( this->multiLineInput );
    vb->addWidget( this->voiceButton );
    vb->addWidget( this->imageButton );
    hbox->addLayout( vb );

    // add everything to the window
    layout->addWidget ( this->historyButton );
    layout->addWidget ( this->messageList );
    layout->addLayout ( hbox );

    this->setLayout ( layout );
    this->resize( 492, 323 );

    if( this->target->getPendingMessages().size() > 0 )
    {
        this->getMessages();
    }

    connect( this->target, SIGNAL( insertMessage() ), this, SLOT( getMessages() ) );
}

ChatWindow::~ChatWindow()
{
    tools_->removeChatWindow ( this->target );
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
    Message m;

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg");

    m.setPayload( buffer.data() );
    m.setType( "image/jpeg" );
    m.setSeconds( QDateTime::currentDateTime().toTime_t() );
    m.setUseconds( 0 );
    m.setSenderID ( tools_->user->userID );
    m.setHist( false );

    this->sendMessage( m );
}

void ChatWindow::sendTextMessage( QString message )
{
    Message m;

    m.setType( "text/plain" );
    m.setPayload( message.toUtf8() );
    m.setSeconds( QDateTime::currentDateTime().toTime_t() );
    m.setUseconds( 0 );
    m.setSenderID ( tools_->user->userID );
    m.setHist( false );

    this->sendMessage( m );
}

void ChatWindow::sendMessage( Message message )
{
    tools_->sendMessage( this->target, message );
    this->messageList->addMessage( message );
}

void ChatWindow::checkMessageInput()
{

    QString message = this->multiLineInput->toPlainText().toUtf8();
    this->multiLineInput->setPlainText("");
    this->sendTextMessage( message );
}

void ChatWindow::appendMessage( Message message )
{
    this->messageList->addMessage( message );
}

void ChatWindow::askForHistory()
{
    Message m = this->messageList->getMessageAt( 0 );
    QString timestamp = QString::number( m.seconds() ) + "." + QString::number( m.useconds() );
    tools_->getHistoryMessage( this->target, timestamp );
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
