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
#include "listviewcontainer.h"

ListViewContainer::ListViewContainer( QWidget *parent, QString name )
 : QWidget(parent)
{
    this->name = name;
    this->downarrow = tools_->getPixmap( ":/misc/downarrow.png" );
    this->rightarrow = tools_->getPixmap( ":/misc/rightarrow.png" );
}

void ListViewContainer::appendWidget( ListItem*, bool )
{
    qDebug( "ListViewContainer::appendWidget - unimplemented" );
}

int ListViewContainer::hasWidget( ListItem* )
{
    qDebug( "ListViewContainer::hasWidget - unimplemented" );
    return 0;
}

void ListViewContainer::removeWidget( ListItem* )
{
    qDebug( "ListViewContainer::removeWidget - unimplemented" );
}

QString ListViewContainer::getName()
{
    return this->name;
}
