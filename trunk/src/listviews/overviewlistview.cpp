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
#include "../listitems/serviceitem.h"
#include "../listviews/overviewlistview.h"
#include "../services/palringoservice.h"

OverviewListView::OverviewListView(QWidget *parent)
 : PalringoListView(parent)
{
}

void OverviewListView::setupContainers()
{
    this->addContainer( tr( "Services" ) );
    this->addContainer( tr( "Messages" ) );

    PalringoService *s = new PalringoService;
    s->Nickname = QString("Palringo");
    s->Status = QString("Offline");
    s->OnlineStatus = "Offline";
    s->Type = "palringo";
    s->Group = "Services";

    this->serviceReceived( s );
    this->addLayoutsToSelf();
}

void OverviewListView::serviceReceived( Service *service )
{
    ServiceItem *si = new ServiceItem( 0, service, true );
    this->listItems.append( si );
    this->addWidgetToView( si );
}

OverviewListView::~OverviewListView()
{
}
