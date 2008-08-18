//
// C++ Interface: listitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

class ListItem : public QWidget
{
    Q_OBJECT
    public:
        ListItem( QWidget *parent = 0 );
        ~ListItem();

        virtual QString getContainerGroup();

        bool getToSelect();
        void setSelected( bool selected );

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
        QLabel *iconLabel;
        QPixmap *icon;
        virtual void ReloadStyleSheet();

    private:
        bool toSelect;
        bool selected;

        QWidget *parent;
};

#endif
