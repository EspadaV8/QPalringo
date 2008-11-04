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
#ifndef GROUPLISTITEM_H
#define GROUPLISTITEM_H

#include <QWidget>
#include "listitem.h"
#include "targets/group.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class GroupListItem : public ListItem
{
    Q_OBJECT
    public:
        GroupListItem( QWidget *parent = 0, Group *group = NULL );
        ~GroupListItem();

        virtual QString getContainerGroup();

    private:
        Group *group;
        QAction *groupChatAction;
        QAction *leaveGroupAction;
        
    private slots:
        void startChat();
        void leaveGroup();

    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );
        virtual void setMenu();

};

#endif
