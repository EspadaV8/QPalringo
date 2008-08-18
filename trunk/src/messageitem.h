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

struct Message
{
    QString Type;
    QByteArray Payload;
    QString Sender;
    QString Timestamp;
};

class MessageItem : public QWidget
{
    Q_OBJECT
    public:
        MessageItem ( QWidget *parent = 0, Message *message = NULL );
        ~MessageItem();

    private:
        Message *message;

        QLabel *messageIcon;
        QLabel *sender;
        QLabel *timestamp;
        QLabel *messageText;

        QHBoxLayout *layout;
        QVBoxLayout *leftside;
        QVBoxLayout *rightside;
        QHBoxLayout *headers;
};

#endif
