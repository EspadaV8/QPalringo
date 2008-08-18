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

ServiceItem::ServiceItem( QWidget *parent, Service *service )
 : ListItem( parent )
{
    this->service = service;
    this->service->Group = "Services";

    setFirstLine( this->service->Nickname );
    setSecondLine( this->service->OnlineStatus + " - " + this->service->Status );
    setIcon( ":/services/" + this->service->Type + "Service.png" );
}

void ServiceItem::setServiceType( QString type )
{
    this->service->Type = type;
    setIcon( ":/services/" + type + "Service.png" );
}

void ServiceItem::setServiceOnlineStatus( QString onlinestatus )
{
    this->service->OnlineStatus = onlinestatus;
    setSecondLine( this->service->OnlineStatus + " - " + this->service->Status );
}

void ServiceItem::setServiceNickname( QString nickname )
{
    this->service->Nickname = nickname;
    setFirstLine( this->service->Nickname );
}

void ServiceItem::setServiceStatus( QString status )
{
    this->service->Status = status;
}

QString ServiceItem::getContainerGroup()
{
    return this->service->Group;
}

void ServiceItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    // qDebug( "Double click" );
    // this->doubleClicked = true;
    emit( doubleClick() );
    event->accept();
}

ServiceItem::~ServiceItem()
{
}


