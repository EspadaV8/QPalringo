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
#ifndef LISTITEM_H
#define LISTITEM_H
#define MAX_ICON_SIZE 24

/**
 @author Andrew Smith <espadav8@gmail.com>
 */

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QStyleOption>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QSvgWidget>
#include <QMenu>

class ListItem : public QFrame
{
    Q_OBJECT
    public:
        ListItem( QWidget *parent = 0 );
        ~ListItem();

        static const int UNDEFINED = 0;
        static const int CONTACT = 1;
        static const int GROUP = 2;
        static const int SERVICE = 3;
        static const int BRIDGECONTACT = 4;

        virtual QString getContainerGroup();

        bool getToSelect();
        void setSelected( bool selected );

        QString getFirstLine();
        int getType();

    public slots:
        void setFirstLine( QString text );
        void setSecondLine( QString text );
        void setIcon( QString iconFilename );
        void removeSelf();

    signals:
        void doubleClick();
        void removeListItem( ListItem* listitem );

    protected:
        void paintEvent( QPaintEvent *event );
        void mousePressEvent( QMouseEvent *event );
        void mouseDoubleClickEvent( QMouseEvent *event );
        void contextMenuEvent( QContextMenuEvent *event );

        QLabel *firstLine;
        QLabel *secondLine;
        QSvgWidget *svgIcon;
        QMenu *popupMenu;
        int type;

        virtual void ReloadStyleSheet();
        virtual void setMenu();

    private:
        bool toSelect;
        bool selected;
};

#endif
