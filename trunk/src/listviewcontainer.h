//
// C++ Interface: listviewcontainer
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTVIEWCONTAINER_H
#define LISTVIEWCONTAINER_H

#include <QtGui>
#include <QWidget>
#include "listitem.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class ListViewContainer : public QWidget
{
Q_OBJECT
public:
    ListViewContainer( QWidget *parent = 0, QString name = "" );

    ~ListViewContainer();

    void appendWidget( ListItem *widget );
    void removeWidget( ListItem *widget );
    QString getName();

    private slots:
        void buttonClicked();

    protected:
        void paintEvent( QPaintEvent *event );

    private:
        QVBoxLayout *outerLayout;
        QVBoxLayout *innerLayout;

        QWidget *layoutContainer;

        QPixmap *downarrow;
        QPixmap *rightarrow;

        QPushButton *button;

        QString name;
};

#endif
