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
#include "qptools.h"
#include "qpgroupcapabilities.h"
#include <QDebug>

ContactListItem::ContactListItem( Contact *contact, qint32 capabilities, QWidget *parent )
    : TargetListItem( contact, parent )
{
    this->contact = contact;
    this->type = ListItem::CONTACT;
    this->setMenu();
    this->capabilities = capabilities;

    if( this->contact->getPendingMessages().size() > 0 )
    {
        this->pendingMessage();
    }
    else
    {
        this->resetDetails();
    }

    connect( contact, SIGNAL( updateNickname() ), this, SLOT( updateNickname() ) );
    connect( contact, SIGNAL( updateStatusline() ), this, SLOT( updateStatusLine() ) );
    connect( contact, SIGNAL( updateOnlineStatus() ), this, SLOT( setContactOnlineStatus() ) );
    connect( contact, SIGNAL( pendingMessage() ), this, SLOT( pendingMessage() ) );
    connect( contact, SIGNAL( clearedPendingMessages() ), this, SLOT( resetDetails() ) );
}

void ContactListItem::updateNickname()
{
    this->setFirstLine( this->contact->getNickname() );
}

void ContactListItem::updateExtraDetails()
{
    switch( this->capabilities )
    {
        case qpGroupCapabilities::ADMIN:
            this->setExtraDetails( "<font color=\"red\">(Admin)</span>" );
            break;
        case qpGroupCapabilities::MOD:
            this->setExtraDetails( "<font color=\"green\">(Mod)</span>" );
            break;
        case qpGroupCapabilities::BANNED:
            this->setExtraDetails( "<font color=\"grey\">(Banned)</span>" );
            break;
        case qpGroupCapabilities::SILENCED:
            this->setExtraDetails( "<font color=\"grey\">(Silenced)</span>" );
            break;
        case qpGroupCapabilities::HIDDEN:
            this->setExtraDetails( "<font color=\"grey\">(Hidden)</span>" );
            break;
        default:
            this->setExtraDetails( "" );
            break;
    }
}

void ContactListItem::updateStatusLine()
{
    this->setSecondLine( this->contact->getStatusline() );
}

void ContactListItem::setContactOnlineStatus()
{
    this->setIcon( QPTools::getTargetIcon( this->contact ) );
    emit( containerGroupChanged( this ) );
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
    emit focusChatWindow( this->contact );
}

void ContactListItem::showContactProperties()
{
    //tools_->showContactProperties( this->contact );
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
    this->updateNickname();
    this->setSecondLine( QString::number( number_of_messages ) + " private message(s)" );
}

void ContactListItem::resetDetails()
{
    this->updateNickname();
    this->updateStatusLine();
    this->updateExtraDetails();
    this->setIcon( QPTools::getTargetIcon( this->contact ) );
}
