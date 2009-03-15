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
#include "grouplistitem.h"
#include "tools.h"

GroupListItem::GroupListItem( QWidget *parent, Group *group )
    : ListItem( parent )
{
    this->group = group;

    this->setIcon( ":/svg/group.svg" );
    this->setFirstLine( this->group->getName() );
    this->setSecondLine( this->group->getDescription() );
    
    this->setMenu();
}

QString GroupListItem::getContainerGroup()
{
    return "Group Chat";
}

void GroupListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    event->accept();
    this->startChat();
}

void GroupListItem::startChat()
{
    tools_->openChatWindow( this->group );
}

void GroupListItem::setMenu()
{
    this->groupChatAction = new QAction( tr( "Group Chat" ), this );
    this->groupChatAction->setStatusTip( tr( "Start a group chat" ) );
    connect( this->groupChatAction, SIGNAL( triggered( bool ) ), this, SLOT( startChat() ) );
    
    this->leaveGroupAction = new QAction( tr( "Leave Group" ), this );
    this->leaveGroupAction->setStatusTip( tr( "Leave the selected group" ) );
    connect( this->leaveGroupAction, SIGNAL( triggered( bool ) ), this, SLOT( leaveGroup() ) );
    
    this->popupMenu->addAction( this->groupChatAction );
    this->popupMenu->addAction( this->leaveGroupAction );
}

void GroupListItem::leaveGroup()
{
    tools_->leaveGroup( this->group->getID() );
}

GroupListItem::~GroupListItem()
{
}
