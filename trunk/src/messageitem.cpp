//
// C++ Implementation: messageitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "messageitem.h"

MessageItem::MessageItem( QWidget *parent, Message *message ) :
        QWidget( parent )
{
    this->message = message;

    this->messageIcon = new QLabel( "" );

    this->sender = new QLabel( this->message->Sender );
    this->sender->setAlignment( Qt::AlignLeft );

    this->timestamp = new QLabel( this->message->Timestamp );
    this->timestamp->setAlignment( Qt::AlignRight );

    QString messageTypeIcon;
    if( this->message->Type == "" )
    {
    }
    else
    {
        this->messageText = new QLabel( QString( this->message->Payload ) );
        messageTypeIcon = ":/messageTypes/text.png";
    }
    
    QPixmap *p = new QPixmap( messageTypeIcon );
    this->messageIcon->setPixmap( *p );
    this->layout = new QHBoxLayout();

    this->leftside = new QVBoxLayout();
    this->rightside = new QVBoxLayout();
    this->headers = new QHBoxLayout();

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
}


MessageItem::~MessageItem()
{
}


