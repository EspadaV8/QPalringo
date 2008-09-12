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
#include <QScrollBar>

MessageList::MessageList(QWidget *parent)
    : QScrollArea(parent)
{
    this->vbox = new QVBoxLayout();
    this->vbox->addStretch( 1 );
    QWidget *w = new QWidget();
    w->setLayout( vbox );
    this->setWidgetResizable( true );
    this->setWidget( w );

    this->atBottom = true;
}


MessageList::~MessageList()
{
}

void MessageList::addMessage( Message *message )
{
    this->atBottom = ( this->verticalScrollBar()->sliderPosition() == this->verticalScrollBar()->maximum() );

    MessageItem *m = new MessageItem( this, message );
    this->messages.append( m );
    this->vbox->insertWidget( ( this->vbox->count() - 1 ), m );
    m->show();

    QMetaObject::invokeMethod(this, "setScrollbarPos", Qt::QueuedConnection);
}

void MessageList::setScrollbarPos()
{
    if( atBottom )
    {
        this->verticalScrollBar()->setSliderPosition( this->verticalScrollBar()->maximum() );
        this->horizontalScrollBar()->setSliderPosition( 0 );
    }
}
