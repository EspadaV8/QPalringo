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
#include "serviceitem.h"

ServiceItem::ServiceItem( Service *service, bool isPalringoService, QWidget *parent )
 : ListItem( parent )
{
    this->service = service;
    this->isPalringoService = isPalringoService;

    setFirstLine( this->service->getNickname() + " - " + this->service->getStatus() );
    setSecondLine( qpOnlineStatus::OnlineStatusText[ this->service->getOnlineStatus() ] );
    setIcon( this->service->getIcon() );
    
    connect( service, SIGNAL( serviceUpdated() ), this, SLOT( updateDetails() ) );
    
    this->setMenu();
}

void ServiceItem::updateDetails()
{
    setIcon( this->service->getIcon() );
    setFirstLine( this->service->getNickname() + " - " + this->service->getStatus() );
    setSecondLine( qpOnlineStatus::OnlineStatusText[ this->service->getOnlineStatus() ] );
}

void ServiceItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    emit doubleClick( this->service );
    event->accept();
}

void ServiceItem::setMenu()
{
    this->editServiceMenuAction = new QAction( tr( "Edit service" ), this );
    this->editServiceMenuAction->setStatusTip( tr( "Edit the selected service" ) );
    //connect( this->editServiceMenuAction, SIGNAL( triggered( bool ) ), this, SLOT( startChat() ) );
    
    this->signonMenuAction = new QAction( tr( "Sign in..." ), this );
    this->signonMenuAction->setStatusTip( tr( "Sign in to the selected service" ) );
    connect( this->signonMenuAction, SIGNAL( triggered( bool ) ), service, SLOT( loginService() ) );


    this->onlineStatus = new QAction( tr( "Online" ), this );
    this->onlineStatus->setStatusTip( tr( "Set status as online" ) );
    connect( this->onlineStatus, SIGNAL( triggered( bool ) ), service, SLOT( changeOnlineStatus() ) );

    this->awayStatus = new QAction( tr( "Away" ), this );
    this->awayStatus->setStatusTip( tr( "Set status as away" ) );
    connect( this->awayStatus, SIGNAL( triggered( bool ) ), service, SLOT( changeOnlineStatus() ) );

    this->busyStatus = new QAction( tr( "Busy" ), this );
    this->busyStatus->setStatusTip( tr( "Set status as busy" ) );
    connect( this->busyStatus, SIGNAL( triggered( bool ) ), service, SLOT( changeOnlineStatus() ) );

    this->invisibleStatus = new QAction( tr( "Invisible" ), this );
    this->invisibleStatus->setStatusTip( tr( "Set status as invisible" ) );
    connect( this->invisibleStatus, SIGNAL( triggered( bool ) ), service, SLOT( changeOnlineStatus() ) );
    
    this->popupMenu->addAction( this->editServiceMenuAction );
    this->popupMenu->addAction( this->signonMenuAction );

    this->popupMenu->addSeparator();

    this->popupMenu->addAction( this->onlineStatus );
    this->popupMenu->addAction( this->awayStatus );
    this->popupMenu->addAction( this->busyStatus );
    this->popupMenu->addAction( this->invisibleStatus );
}

ServiceItem::~ServiceItem()
{
}


