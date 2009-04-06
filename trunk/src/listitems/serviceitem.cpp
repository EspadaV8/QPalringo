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
#include "tools.h"

ServiceItem::ServiceItem( QWidget *parent, Service *service, bool isPalringoService )
 : ListItem( parent )
{
    this->service = service;
    this->isPalringoService = isPalringoService;

    setFirstLine( this->service->Nickname + " - " + this->service->Status );
    setSecondLine( this->service->OnlineStatus );
    setIcon( ":/svg/" + this->service->Type + "Service.svg" );
    
    connect( this, SIGNAL( doubleClick() ), service, SLOT( loginService() ) );
    connect( service, SIGNAL( serviceUpdated() ), this, SLOT( updateDetails() ) );
    
    this->setMenu();
}

void ServiceItem::updateDetails()
{
    setIcon( ":/svg/" + this->service->Type + "Service.svg" );
    setFirstLine( this->service->Nickname + " - " + this->service->OnlineStatus );
    setSecondLine( this->service->Status );
}

void ServiceItem::changeOnlineStatus()
{
    QObject* sender = QObject::sender();
    QAction* action = qobject_cast<QAction *>(sender);

    if( action != 0 )
    {
        QString status;
        if( action->text() == "Online" )
        {
            status = "1";
        }
        else if( action->text() == "Away" )
        {
            status = "2";
        }
        else if( action->text() == "Invisible" )
        {
            status = "3";
        }
        else if( action->text() == "Busy" )
        {
            status = "5";
        }

        tools_->updateContactDetail( "ONLINE-STATUS", status );
    }
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
    connect( this->signonMenuAction, SIGNAL( triggered( bool ) ), service, SLOT( loginService() ) );


    this->onlineStatus = new QAction( tr( "Online" ), this );
    this->onlineStatus->setStatusTip( tr( "Set status as online" ) );
    connect( this->onlineStatus, SIGNAL( triggered( bool ) ), this, SLOT( changeOnlineStatus() ) );

    this->awayStatus = new QAction( tr( "Away" ), this );
    this->awayStatus->setStatusTip( tr( "Set status as away" ) );
    connect( this->awayStatus, SIGNAL( triggered( bool ) ), this, SLOT( changeOnlineStatus() ) );

    this->busyStatus = new QAction( tr( "Busy" ), this );
    this->busyStatus->setStatusTip( tr( "Set status as busy" ) );
    connect( this->busyStatus, SIGNAL( triggered( bool ) ), this, SLOT( changeOnlineStatus() ) );

    this->invisibleStatus = new QAction( tr( "Invisible" ), this );
    this->invisibleStatus->setStatusTip( tr( "Set status as invisible" ) );
    connect( this->invisibleStatus, SIGNAL( triggered( bool ) ), this, SLOT( changeOnlineStatus() ) );
    
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


