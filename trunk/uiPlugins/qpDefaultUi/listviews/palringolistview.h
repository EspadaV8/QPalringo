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
#ifndef PALRINGOLISTVIEW_H
#define PALRINGOLISTVIEW_H

/**
	@author Andrew Smith <espadav8@gmail.com>
*/

#include <QtGui>
#include <QWidget>
#include <QList>
#include "listviewcontainer.h"
#include "../listitems/listitem.h"

class PalringoListView : public QScrollArea
{
    Q_OBJECT
    public:
        PalringoListView( QWidget *parent = 0 );
        ~PalringoListView();

        virtual void setupContainers();

    protected:
        void mousePressEvent( QMouseEvent *event );
        void addContainer( QString containerName );

        void addLayoutsToSelf();
        int  getContainerPosition( QString containerName );
        bool addWidgetToView( ListItem *item );
        bool addWidgetToView( ListItem *item, QString containerName );

        QVBoxLayout *listLayout;

        QList<ListItem *> listItems;
        QList<ListViewContainer *> listViewContainers;

    public slots:
        virtual void inFocus();
        void removeListItem( ListItem *item );
};

#endif
