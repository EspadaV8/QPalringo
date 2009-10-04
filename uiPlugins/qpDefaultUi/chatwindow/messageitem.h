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
#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

/**
 @author Andrew Smith <espadav8@gmail.com>
*/

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QSvgWidget>
#include "message.h"

class MessageItem : public QFrame
{
    Q_OBJECT
    public:
        MessageItem ( QWidget *parent = 0, Message message = Message() );
        ~MessageItem();

        Message getMessage();

    private:
        Message message;

        QSvgWidget *messageIcon;
        QLabel *sender;
        QLabel *timestamp;
        QLabel *messageText;

        QHBoxLayout *layout;
        QVBoxLayout *leftside;
        QVBoxLayout *rightside;
        QHBoxLayout *headers;

        void handleAudioClip();

    signals:
        void doubleClick();

    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );

    private slots:
        void showMetaData();
        void finished();
};

#endif
