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
#include "tools.h"
#include "contactlistview.h"

ContactListView::ContactListView( QWidget *parent )
 : PalringoListView( parent )
{
}

ContactListView::~ContactListView()
{
    foreach( ListItem *pc, this->listItems )
    {
        delete pc;
    }
}

void ContactListView::setupContainers()
{
    this->addContainer( tr( "Online" ) );
    this->addContainer( tr( "Offline" ) );

    connect( tools_, SIGNAL( userContactReceived( Contact* ) ), this, SLOT( contactReceived( Contact* ) ) );

    this->addLayoutsToSelf();
}

void ContactListView::contactReceived( Contact *contact )
{
    ContactListItem *pc = new ContactListItem( this, contact );
    connect( pc, SIGNAL( containerGroupChanged( ContactListItem* ) ), this, SLOT( checkContainerGroups( ContactListItem* ) ) );
    this->listItems.append( pc );
    this->addWidgetToView( pc );
}

void ContactListView::getContacts( quint64 groupID )
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
            this->contactReceived( contact );
        }
    }
    this->setUpdatesEnabled( true );
}

void ContactListView::inFocus()
{
    if( this->contacts.size() == 0 )
    {
        this->getContacts();
    }
}

void ContactListView::checkContainerGroups( ContactListItem *li )
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
