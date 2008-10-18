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
#include "service.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/

class ServiceItem : public ListItem
{
    Q_OBJECT
    public:
        ServiceItem( QWidget *parent = 0, Service *service = NULL, bool isPalringoService = false );
        ~ServiceItem();
        QString getContainerGroup();
    
    private slots:
        void updateDetails();
        
    signals:
        void doubleClick();
        
    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );
        
    private:
        Service *service;
        bool isPalringoService;
};

#endif
