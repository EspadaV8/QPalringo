#ifndef DEFAULTLISTVIEWCONTAINER_H
#define DEFAULTLISTVIEWCONTAINER_H

#include "listviewcontainer.h"

class DefaultListViewContainer : public ListViewContainer
{
    Q_OBJECT
    public:
        DefaultListViewContainer( QWidget *parent = 0, QString name = "" );;

        void appendWidget( ListItem *widget, bool sorted = true );
        void removeWidget( ListItem *widget );
        int hasWidget( ListItem *widget );

    private slots:
        void buttonClicked();

    private:
        QVBoxLayout *outerLayout;
        QVBoxLayout *innerLayout;
        QWidget *layoutContainer;
        QPushButton *button;
};

#endif // DEFAULTLISTVIEWCONTAINER_H
