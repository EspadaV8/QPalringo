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
#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

/**
 @author Andrew Smith <espadav8@gmail.com>
*/

#include <QWidget>
#include <QAction>
#include "listitem.h"
#include "targets/contact.h"

class ContactListItem : public ListItem
{
    Q_OBJECT
    public:
        ContactListItem( QWidget *parent = 0, Contact *contact = NULL );
        ~ContactListItem();

        Contact* getContact();

        QString getContainerGroup();

    public slots:
        void setContactOnlineStatus( int onlinestatus );
        
    private slots:
        void startChat();
        void showContactProperties();

    protected:
        void paintEvent( QPaintEvent *event );
        void mouseDoubleClickEvent( QMouseEvent *event );
        virtual void setMenu();

    private:
        Contact *contact;
        
        QAction *chatMenuAction;
        QAction *propertiesMenuAction;
};

#endif
