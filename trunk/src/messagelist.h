//
// C++ Interface: messagelist
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QScrollArea>
#include <QVBoxLayout>
#include "messageitem.h"

/**
 @author Andrew Smith <espadav8@gmail.com>
*/

class MessageList : public QScrollArea
    {
            Q_OBJECT
        public:
            MessageList ( QWidget *parent = 0 );
            ~MessageList();

            void addMessage ( Message *message );

        private:
            QVBoxLayout *vbox;
            QList<MessageItem *> messages;
            bool atBottom;

        private slots:
            void setScrollbarPos();
    };

#endif
