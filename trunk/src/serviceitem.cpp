//
// C++ Implementation: serviceitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

ServiceItem::~ServiceItem()
{
}


