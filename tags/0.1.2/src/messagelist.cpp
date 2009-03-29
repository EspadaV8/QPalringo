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
#include "messagelist.h"
#include <QScrollBar>

MessageList::MessageList(QWidget *parent)
    : QScrollArea(parent)
{
    this->vbox = new QVBoxLayout;
    this->vbox->addStretch( 1 );
    QWidget *w = new QWidget;
    w->setLayout( vbox );
    this->setWidgetResizable( true );
    this->setWidget( w );

    this->atBottom = true;
}


MessageList::~MessageList()
{
}

void MessageList::addMessage( Message message )
{
    qint64 pos = 0;
    if( this->messages.count() == 0 )
    {
        pos = -1;
    }
    else
    {
        for( pos = 0; pos < this->messages.count(); pos++ )
        {
            MessageItem *mi = (MessageItem*)this->vbox->itemAt( pos )->widget();
            if( mi->getMessage().timestamp() <= message.timestamp() )
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

void MessageList::insertMessage( qint64 pos, Message message )
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
