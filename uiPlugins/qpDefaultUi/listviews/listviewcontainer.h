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
#ifndef LISTVIEWCONTAINER_H
#define LISTVIEWCONTAINER_H

#include <QtGui>
#include "../listitems/listitem.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class ListViewContainer : public QWidget
{
    Q_OBJECT
    public:
        ListViewContainer( QWidget *parent = 0, QString name = "" );

        virtual void appendWidget( ListItem *widget, bool sorted = true );
        virtual void removeWidget( ListItem *widget );
        virtual int hasWidget( ListItem *widget );
        QString getName();

    protected:
        QPixmap downarrow;
        QPixmap rightarrow;
        QString name;
};

#endif
