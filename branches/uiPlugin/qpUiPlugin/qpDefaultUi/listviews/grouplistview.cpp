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
#include "grouplistview.h"
#include "../listitems/grouplistitem.h"

GroupListView::GroupListView(QWidget *parent, Group *group)
 : ContactListView(parent)
{
    this->group = group;
    this->disconnect( tools_, SIGNAL( cleanUp() ), this, 0 );
}

void GroupListView::setupContainers()
{
    this->addContainer( tr( "Group Chat" ) );
    this->addContainer( tr( "Online" ) );
    this->addContainer( tr( "Offline" ) );

    GroupListItem *gli = new GroupListItem( this, this->group );
    this->addWidgetToView( gli );

    connect( tools_, SIGNAL( userContactReceived( Contact* ) ), this, SLOT( contactReceived( Contact* ) ) );
    
    this->addLayoutsToSelf();
}

void GroupListView::getContacts()
{
    quint64 groupID = this->group->getID();
    ContactListView::getContacts( groupID );
}

Group* GroupListView::getGroup()
{
    return this->group;
}

void GroupListView::inFocus()
{
    if( this->contacts.size() == 0 )
    {
        this->getContacts();
    }
}

GroupListView::~GroupListView()
{
}

