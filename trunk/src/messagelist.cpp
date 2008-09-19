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
    qint64 pos = 0;
    if( this->messages.count() == 0 )
    {
        pos = -1;
    }
    else
    {
        for( pos; pos < this->messages.count(); pos++ )
        {
            MessageItem *mi = (MessageItem*)this->vbox->itemAt( pos )->widget();
            if( mi->getMessage().timestamp < message->timestamp )
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    this->insertMessage( pos, message );
}

void MessageList::insertMessage( qint64 pos, Message *message )
{
    MessageItem *m = new MessageItem( this, message );
    this->atBottom = ( this->verticalScrollBar()->sliderPosition() == this->verticalScrollBar()->maximum() );

    if( pos == -1 )
    {
        this->messages.append( m );
        this->vbox->insertWidget( this->vbox->count() - 1, m );
    }
    else
    {
        this->messages.insert( pos, m );
        this->vbox->insertWidget( pos, m );
    }
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
