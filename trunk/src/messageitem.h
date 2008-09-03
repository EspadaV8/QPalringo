//
// C++ Interface: messageitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

/**
 @author Andrew Smith <espadav8@gmail.com>
*/

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include "message.h"
#include "contact.h"

class MessageItem : public QWidget
{
    Q_OBJECT
    public:
        MessageItem ( QWidget *parent = 0, Message *message = NULL );
        ~MessageItem();

        bool getToSelect();
        void setSelected( bool selected );

    private:
        Message *message;
        Contact* contact;

        QLabel *messageIcon;
        QLabel *sender;
        QLabel *timestamp;
        QLabel *messageText;

        QHBoxLayout *layout;
        QVBoxLayout *leftside;
        QVBoxLayout *rightside;
        QHBoxLayout *headers;

        bool toSelect;
        bool selected;

    signals:
        void doubleClick();

    protected:
        void paintEvent( QPaintEvent *event );
        void mousePressEvent( QMouseEvent *event );
        void mouseDoubleClickEvent( QMouseEvent *event );
        virtual void ReloadStyleSheet();
};

#endif
