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
#include "contactlistitem.h"
#include "tools.h"

ContactListItem::ContactListItem( QWidget *parent, Contact *contact )
    : ListItem( parent )
{
    this->contact = contact;
    this->type = ListItem::CONTACT;
    this->resetDetails();
    this->setMenu();

    if( this->contact->getPendingMessages().size() > 0 )
    {
        this->pendingMessage();
    }

    connect( contact, SIGNAL( updateNickname( QString ) ), this, SLOT( setFirstLine( QString ) ) );
    connect( contact, SIGNAL( updateStatusline( QString ) ), this, SLOT( setSecondLine( QString ) ) );
    connect( contact, SIGNAL( updateOnlineStatus( int ) ), this, SLOT( setContactOnlineStatus( int ) ) );
    connect( contact, SIGNAL( pendingMessage() ), this, SLOT( pendingMessage() ) );
    connect( contact, SIGNAL( clearedPendingMessages() ), this, SLOT( resetDetails() ) );
}

void ContactListItem::setContactOnlineStatus( int onlinestatus __attribute__ ((unused)) )
{
    this->setIcon( this->contact->getIcon() );
}

Contact* ContactListItem::getContact()
{
    return this->contact;
}

QString ContactListItem::getContainerGroup()
{
    if( this->contact->getOnlineStatus() > 0 )
    {
        return "Online";
    }
    else
    {
        return "Offline";
    }
}

void ContactListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    event->accept();
    this->startChat();
}

void ContactListItem::startChat()
{
    tools_->openChatWindow( this->contact );
}

void ContactListItem::showContactProperties()
{
    tools_->showContactProperties( this->contact );
}

void ContactListItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ContactListItem::setMenu()
{
    this->chatMenuAction = new QAction( tr( "Private Chat" ), this );
    this->chatMenuAction->setStatusTip( tr( "Start a private chat" ) );
    connect( this->chatMenuAction, SIGNAL( triggered( bool ) ), this, SLOT( startChat() ) );
    
    this->propertiesMenuAction = new QAction( tr( "Properties" ), this );
    this->propertiesMenuAction->setStatusTip( tr( "View users properties" ) );
    connect( this->propertiesMenuAction, SIGNAL( triggered( bool ) ), this, SLOT( showContactProperties() ) );
    
    this->popupMenu->addAction( this->chatMenuAction );
    this->popupMenu->addAction( this->propertiesMenuAction );
}

ContactListItem::~ContactListItem()
{
    delete this->chatMenuAction;
    delete this->propertiesMenuAction;
}

void ContactListItem::pendingMessage()
{
    int number_of_messages = this->contact->getPendingMessages().size();
    this->setIcon( ":/svg/text.svg" );
    this->setSecondLine( QString::number( number_of_messages ) + " unread message(s)" );
}

void ContactListItem::resetDetails()
{
    this->setFirstLine( this->contact->getNickname() );
    this->setSecondLine( this->contact->getStatusline() );
    this->setIcon( this->contact->getIcon() );
}
