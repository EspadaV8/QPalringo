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
#ifndef GROUPLISTVIEW_H
#define GROUPLISTVIEW_H

#include "contactlistview.h"
#include "../libQPalringo/targets/group.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class GroupListView : public ContactListView
{
    Q_OBJECT
    public:
        GroupListView(QWidget *parent = 0, Group *group = 0);
        ~GroupListView();

        virtual void setupContainers();

    private:
        Group *group;

    protected slots:
        void getContacts();

    public slots:
        virtual void inFocus();

};

#endif
