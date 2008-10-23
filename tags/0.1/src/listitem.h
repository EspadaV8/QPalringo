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

class ListItem : public QWidget
{
    Q_OBJECT
    public:
        ListItem( QWidget *parent = 0 );
        ~ListItem();

        virtual QString getContainerGroup();

        bool getToSelect();
        void setSelected( bool selected );

        QString getFirstLine();

    public slots:
        void setFirstLine( QString text );
        void setSecondLine( QString text );
        void setIcon( QString iconFilename );

    signals:
        void doubleClick();

    protected:
        void paintEvent( QPaintEvent *event );
        void mousePressEvent( QMouseEvent *event );
        void mouseDoubleClickEvent( QMouseEvent *event );

        QLabel *firstLine;
        QLabel *secondLine;
        QSvgWidget *svgIcon;
        virtual void ReloadStyleSheet();

    private:
        bool toSelect;
        bool selected;

        QWidget *parent;
};

#endif
