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
#include "contactlistview.h"

ContactListView::ContactListView( QWidget *parent, Tools* tools_ )
 : PalringoListView( parent, tools_ )
{
    connect( tools_, SIGNAL( cleanUp() ), this, SLOT( removeContacts() ) );
}

ContactListView::~ContactListView()
{
    removeContacts();
}

void ContactListView::removeContacts()
{
    foreach( ListItem *pc, this->listItems )
    {
        delete pc;
    }
    this->contacts.clear();
}

void ContactListView::setupContainers()
{
    this->addContainer( tr( "Online" ) );
    this->addContainer( tr( "Offline" ) );

    connect( tools_, SIGNAL( connected() ), this, SLOT( getContacts() ) );
    connect( tools_, SIGNAL( userContactReceived( Contact* ) ), this, SLOT( contactReceived( Contact* ) ) );

    this->addLayoutsToSelf();
}

void ContactListView::contactReceived( Contact *contact, qint32 capabilities )
{
    ContactListItem *pc = new ContactListItem( contact, capabilities );
    connect( pc, SIGNAL( containerGroupChanged( ListItem* ) ), this, SLOT( checkContainerGroups( ListItem* ) ) );
    this->listItems.append( pc );
    this->addWidgetToView( pc );
}

void ContactListView::bridgeContactReceived( BridgeContact *contact )
{
    BridgeContactListItem *pc = new BridgeContactListItem( contact );
    connect( pc, SIGNAL( containerGroupChanged( ListItem* ) ), this, SLOT( checkContainerGroups( ListItem* ) ) );
    this->listItems.append( pc );
    this->addWidgetToView( pc );
}

void ContactListView::getContacts( quint64 groupID )
{
    if( tools_->isLoggedIn() )
    {
        if( groupID == 0 )
        {
            this->contacts = tools_->getContactListContacts();
        }
        else
        {
            this->contacts = tools_->getGroupContacts( groupID );
        }

        this->setUpdatesEnabled( false );
        if( this->contacts.size() > 0 )
        {
            foreach( Contact *contact, this->contacts )
            {
                qint32 capabilities = 0;
                if( groupID > 0 )
                    capabilities = tools_->getGroup( groupID )->getContacts().value( contact->getID(), 0 );
                this->contactReceived( contact, capabilities );
            }
        }
        this->setUpdatesEnabled( true );
    }
}

void ContactListView::inFocus()
{
    if( this->contacts.size() == 0 )
    {
        this->getContacts();
    }
}

void ContactListView::checkContainerGroups( ListItem *li )
{
    for( int i = 0; i < this->listViewContainers.size(); i++ )
    {
        ListViewContainer *lvc = this->listViewContainers.at( i );
        if ( lvc->hasWidget( li ) != -1 )
        {
            if( lvc->getName() != li->getContainerGroup() )
            {
                lvc->removeWidget( li );
                this->addWidgetToView( li );
            }
            return;
        }
    }
}
