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
#ifndef SERVICEITEM_H
#define SERVICEITEM_H

#include "listitem.h"
#include "services/service.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/

class ServiceItem : public ListItem
{
    Q_OBJECT
    public:
        ServiceItem( Service *service = NULL, bool isPalringoService = false, QWidget *parent = 0 );
        ~ServiceItem();
    
    private slots:
        void updateDetails();
        
    signals:
        void doubleClick( Service* );
        
    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );
        virtual void setMenu();
        
    private:
        Service *service;
        bool isPalringoService;
        
        QAction *editServiceMenuAction;
        QAction *signonMenuAction;
        QAction *onlineStatus;
        QAction *awayStatus;
        QAction *busyStatus;
        QAction *invisibleStatus;
};

#endif
