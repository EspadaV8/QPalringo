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
#include "serviceitem.h"

ServiceItem::ServiceItem( QWidget *parent, Service *service, bool isPalringoService )
 : ListItem( parent )
{
    this->service = service;
    this->isPalringoService = isPalringoService;

    setFirstLine( this->service->Nickname + " - " + this->service->Status );
    setSecondLine( this->service->OnlineStatus );
    setIcon( ":/svg/" + this->service->Type + "Service.svg" );
    
    connect( service, SIGNAL( serviceUpdated() ), this, SLOT( updateDetails() ) );
    
    this->setMenu();
}

void ServiceItem::updateDetails()
{
    setIcon( ":/svg/" + this->service->Type + "Service.svg" );
    setFirstLine( this->service->Nickname + " - " + this->service->OnlineStatus );
    setSecondLine( this->service->Status );
}

QString ServiceItem::getContainerGroup()
{
    return this->service->Group;
}

void ServiceItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    emit( doubleClick() );
    event->accept();
}

void ServiceItem::setMenu()
{
    this->editServiceMenuAction = new QAction( tr( "Edit service" ), this );
    this->editServiceMenuAction->setStatusTip( tr( "Edit the selected service" ) );
    //connect( this->editServiceMenuAction, SIGNAL( triggered( bool ) ), this, SLOT( startChat() ) );
    
    this->signonMenuAction = new QAction( tr( "Sign in..." ), this );
    this->signonMenuAction->setStatusTip( tr( "Sign in to the selected service" ) );
    //connect( this->signonMenuAction, SIGNAL( triggered( bool ) ), this, SLOT( leaveGroup() ) );
    
    this->popupMenu->addAction( this->editServiceMenuAction );
    this->popupMenu->addAction( this->signonMenuAction );
}

ServiceItem::~ServiceItem()
{
}


