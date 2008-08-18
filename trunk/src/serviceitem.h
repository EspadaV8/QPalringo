//
// C++ Interface: serviceitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SERVICEITEM_H
#define SERVICEITEM_H

#include "listitem.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/

struct Service
{
    QString Type;
    QString OnlineStatus;
    QString Nickname;
    QString Status;
    QString Group;
};

class ServiceItem : public ListItem
{
    Q_OBJECT
    public:
        ServiceItem( QWidget *parent = 0, Service *service = NULL );
        ~ServiceItem();
        
        void setServiceType( QString type );
        void setServiceOnlineStatus( QString onlinestatus );
        void setServiceNickname( QString nickname );
        void setServiceStatus( QString status );
        QString getContainerGroup();
        
    signals:
        void doubleClick();
        
    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );
        
    private:
        Service *service;
};

#endif
