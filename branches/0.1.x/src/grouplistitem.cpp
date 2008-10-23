/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QPalringo. If not, see <http://www.gnu.org/licenses/>       *
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
}

QString GroupListItem::getContainerGroup()
{
    return "Group Chat";
}

void GroupListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    event->accept();
    tools_->openChatWindow( this->group );
}

GroupListItem::~GroupListItem()
{
}
