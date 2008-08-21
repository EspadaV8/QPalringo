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
#include "chatwindow.h"
#include "tools.h"

ChatWindow::ChatWindow ( PalringoWindow *parent, Target *target, bool isGroup )
    : QWidget ( parent )
{
    qDebug ( "in chat window" );
    this->setWindowFlags ( Qt::Window );
    this->parent = parent;
    this->target = target;

    this->setWindowTitle ( this->target->getTitle() );
    // this->setWindowIcon ( *new QPixmap ( this->contact->getContactIcon() ) );
    this->setAttribute ( Qt::WA_DeleteOnClose, true );

    // Create all the layouts
    this->layout = new QVBoxLayout();
    this->hbox = new QHBoxLayout();

    // set up all the widgets
    this->historyButton = new QPushButton ( "History", this );
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

    // add the items to the bottom layout
    this->hbox->addWidget( this->messageInput );
    this->hbox->addWidget( this->voiceButton );
    this->hbox->addWidget( this->imageButton );

    // add everything to the window
    this->layout->addWidget ( this->historyButton );
    this->layout->addWidget ( this->messageList );
    this->layout->addLayout ( this->hbox );

    this->setLayout ( this->layout );
}

ChatWindow::~ChatWindow()
{
    tools_->removeChatWindow ( this->contact );
}

void ChatWindow::checkMessageInput()
{
    QString message = this->messageInput->text();
    qDebug( "Message input has changed - %s", qPrintable( message ) );

    Message *m = new Message;
    m->Payload = message.toUtf8();
    m->Type = "text/plain";
    m->Timestamp = QDateTime::currentDateTime().toString( "hh:mm:ss" );
    m->Sender = "me";

    if( this->contact == NULL )
    {
        tools_->sendMessage( this->group, m );
    }
    else
    {
        tools_->sendMessage( this->contact, m );
    }
    this->messageList->addMessage( m );
    this->messageInput->clear();
}

void ChatWindow::appendMessage( QString message, Contact *contact, QString contentType )
{
    Message *m = new Message;
    m->Payload = message.toUtf8();
    m->Type = contentType;
    m->Timestamp = QDateTime::currentDateTime().toString( "hh:mm:ss" );
    m->Sender = contact->getNickname();

    this->messageList->addMessage( m );
}
