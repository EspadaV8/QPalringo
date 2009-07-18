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
#include "listitems/serviceitem.h"
#include "listitems/targetlistitem.h"
#include "listitems/contactlistitem.h"
#include "listitems/grouplistitem.h"
#include "listitems/bridgecontactlistitem.h"
#include "overviewlistview.h"
#include "services/palringoservice.h"
#include "tools.h"

#include "services/bridgeservice.h"
#include "libQPalringo/onlinestatus.h"

OverviewListView::OverviewListView(QWidget *parent)
 : PalringoListView(parent)
{
}

void OverviewListView::setupContainers()
{
    this->addContainer( tr( "Services" ) );
    this->addContainer( tr( "Messages" ) );

    PalringoService *s = new PalringoService;
    s->setType( qpBridgeType::PALRINGO );
    s->setNickname( "Palringo" );
    s->setStatus( "Offline" );
    s->setOnlineStatus( qpOnlineStatus::OFFLINE );

    connect( tools_, SIGNAL( gotBridgeDetails( Bridge* ) ), this, SLOT( newBridge( Bridge* ) ) );
    connect( tools_, SIGNAL( newPendingMessage( Target* ) ), this, SLOT( newPendingMessage( Target* ) ) );

    this->serviceReceived( s );
    this->addLayoutsToSelf();
}

void OverviewListView::newBridge( Bridge* bridge )
{
    Service *s;
    switch( bridge->getType() )
    {
        case qpBridgeType::PALRINGO:
            s = new Service();
            break;
        default:
            s = new BridgeService( bridge );
            break;
    }
    s->setNickname( bridge->getNickname() );
    s->setStatus( bridge->getUsername() );
    s->setOnlineStatus( qpOnlineStatus::OFFLINE );
    s->setType( bridge->getType() );

   this->serviceReceived( s );
}

void OverviewListView::serviceReceived( Service *service )
{
    ServiceItem *si = new ServiceItem( service, true );
    this->listItems.append( si );
    this->addWidgetToView( si, "Services" );
}

void OverviewListView::newPendingMessage( Target* target )
{
    if( !tools_->checkChatWindowOpen( target ) )
    {
        if( this->knownTargets.contains( target ) )
        {
            foreach( ListItem* li, this->listItems )
            {
                TargetListItem* tli = qobject_cast<TargetListItem*>( li );
                if( tli && tli->getTarget() == target )
                {
                    li->show();
                    break;
                }
            }
        }
        else
        {
            ListItem* li;
            int targetType = target->getType();
            switch( targetType )
            {
                case Target::CONTACT:
                    li = new ContactListItem( (Contact*)target );
                    break;
                case Target::GROUP:
                    li = new GroupListItem( (Group*)target );
                    break;
                case Target::BRIDGECONTACT:
                    li = new BridgeContactListItem( (BridgeContact*)target );
                    break;
                default:
                    return;
            }

            this->listItems.append( li );
            this->addWidgetToView( li, "Messages" );
            this->knownTargets.append( target );

            connect( target, SIGNAL( clearedPendingMessages() ), li, SLOT( removeSelf() ) );
            connect( li, SIGNAL( removeListItem( ListItem* ) ), this, SLOT( removeListItem( ListItem* ) ) );
        }
    }
}

OverviewListView::~OverviewListView()
{
}
