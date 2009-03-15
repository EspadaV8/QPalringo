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
#include "../listitems/contactlistitem.h"

ContactListView::ContactListView( QWidget *parent )
 : PalringoListView( parent )
{
}

ContactListView::~ContactListView()
{
}

void ContactListView::setupContainers()
{
    this->addContainer( tr( "Online" ) );
    this->addContainer( tr( "Offline" ) );
    
    connect( tools_, SIGNAL( connected() ), this, SLOT( getContacts() ) );
    connect( tools_, SIGNAL( userContactReceived( Contact* ) ), this, SLOT( contactReceived( Contact* ) ) );

    this->addLayoutsToSelf();
}

void ContactListView::contactReceived( Contact *contact )
{
    ContactListItem *pc = new ContactListItem( this, contact );
    this->listItems.append( pc );
    this->addWidgetToView( pc );
}

void ContactListView::getContacts( quint64 groupID )
{
    QHash<quint64, Contact*> contacts = tools_->getContacts( groupID );
    
    this->setUpdatesEnabled( false );
    if( contacts.size() > 0 )
    {
        foreach( Contact *contact, contacts )
        {
            this->contactReceived( contact );
        }
    }
    this->setUpdatesEnabled( true );
}
