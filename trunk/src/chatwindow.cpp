//
// C++ Implementation: chatwindow
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QFileDialog>
#include "chatwindow.h"
#include "tools.h"

ChatWindow::ChatWindow ( PalringoWindow *parent, Target *target, bool isGroup )
    : QWidget ( parent )
{
    qDebug ( "in chat window" );
    this->setWindowFlags ( Qt::Window );
    this->parent = parent;
    this->target = target;
    this->isGroup = isGroup;
    this->earliestTimestamp = QDateTime::currentDateTime();

    this->setWindowTitle( this->target->getTitle() );
    // this->setWindowIcon ( *new QPixmap ( this->contact->getContactIcon() ) );
    this->setAttribute ( Qt::WA_DeleteOnClose, true );

    // Create all the layouts
    this->layout = new QVBoxLayout();
    this->hbox = new QHBoxLayout();

    // set up all the widgets
    this->historyButton = new QPushButton ( "History", this );
    connect( this->historyButton, SIGNAL( clicked() ), this, SLOT( askForHistory() ) );
    this->messageList = new MessageList( this );
    this->messageInput = new QLineEdit( this );
    this->voiceButton = new QPushButton ( "" );
    this->imageButton = new QPushButton ( "" );

    // connect to the return key, so we can send a message
    connect( this->messageInput, SIGNAL( returnPressed() ), this, SLOT(checkMessageInput()) );

    // add the icons to the buttons
    QPixmap *p = new QPixmap( ":/messageTypes/voice.png" );
    this->voiceButton->setIcon( *p );
    this->voiceButton->setToolTip( "Press and hold to record voice message" );
    p = new QPixmap( ":/messageTypes/image.png" );
    this->imageButton->setIcon( *p );
    connect( this->imageButton, SIGNAL( clicked() ), this, SLOT( loadImageFile() ) );

    // add the items to the bottom layout
    this->hbox->addWidget( this->messageInput );
    this->hbox->addWidget( this->voiceButton );
    this->hbox->addWidget( this->imageButton );

    // add everything to the window
    this->layout->addWidget ( this->historyButton );
    this->layout->addWidget ( this->messageList );
    this->layout->addLayout ( this->hbox );

    this->setLayout ( this->layout );
    this->resize( 492, 323 );
}

ChatWindow::~ChatWindow()
{
    tools_->removeChatWindow ( this->target );
}

void ChatWindow::loadImageFile()
{
    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.gif)"));
    
    if( fileName != "" )
    {
        QImage *image = new QImage( fileName );
        
        if ( image )
        {
            Message *m = new Message;
            
            
            QBuffer buffer(&m->payload);
            buffer.open(QIODevice::WriteOnly);
            image->save(&buffer, "jpg");
            
            m->type = "image/jpeg";
            m->timestamp = QDateTime::currentDateTime();
            m->senderID = tools_->user->userID;

            tools_->sendMessage( this->target, this->isGroup, m );
            this->messageList->addMessage( m );
        }
    }
    
}

void ChatWindow::checkMessageInput()
{
    QString message = this->messageInput->text().toUtf8();
    qDebug( "Message input has changed - %s", qPrintable( message ) );

    Message *m = new Message;
    m->payload.append( message );
    m->type = "text/plain";
    m->timestamp = QDateTime::currentDateTime();
    m->senderID = tools_->user->userID;

    tools_->sendMessage( this->target, this->isGroup, m );
    this->messageList->addMessage( m );
    this->messageInput->clear();
}

void ChatWindow::appendMessage( QString message, Contact *contact, QString contentType )
{
    Message *m = new Message;
    m->payload = message.toUtf8();
    m->type = contentType;
    m->timestamp = QDateTime::currentDateTime();
    m->senderID = 1; //contact->getNickname();

    this->messageList->addMessage( m );
}

void ChatWindow::appendMessage( Message* message )
{
    if( message->timestamp < this->earliestTimestamp )
    {
        this->earliestTimestamp = message->timestamp;
    }
    this->messageList->addMessage( message );
}

void ChatWindow::askForHistory()
{
    qDebug( "time to get some history" );
    tools_->getHistoryMessage( this->target, this->isGroup,  this->earliestTimestamp );
}
