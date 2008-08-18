//
// C++ Implementation: messagelist
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "messagelist.h"

MessageList::MessageList(QWidget *parent)
    : QScrollArea(parent)
{
    this->vbox = new QVBoxLayout();
    this->vbox->addStretch( 1 );
    QWidget *w = new QWidget();
    w->setLayout( vbox );
    this->setWidgetResizable( true );
    this->setWidget( w );
}


MessageList::~MessageList()
{
}

void MessageList::addMessage( Message *message )
{
    MessageItem *m = new MessageItem( this, message );
    this->messages.append( m );
    this->vbox->insertWidget( ( this->vbox->count() - 1 ), m );
}
