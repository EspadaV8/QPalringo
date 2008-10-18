//
// C++ Implementation: service
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "service.h"

Service::Service(QObject *parent)
 : QObject(parent)
{
}

void Service::loggedOn()
{
    qDebug( "Not implemented" );
}

Service::~Service()
{
}
