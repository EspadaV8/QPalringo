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
#ifndef MESSAGE_H
#define MESSAGE_H

#include <QMetaType>
#include <QString>
#include <QByteArray>
#include <QDateTime>
/**
	@author Andrew Smith <espadav8@gmail.com>
*/
struct Message
{
    QString type;
    QByteArray payload;
    quint64 senderID;
    quint64 groupID;
    QDateTime timestamp;
    bool hist;

    Message()
    {
        qRegisterMetaType<Message>("Message");
    }

    Message(const Message& other)
    {
        this->type = other.type;
        this->payload = other.payload;
        this->senderID = other.senderID;
        this->groupID = other.groupID;
        this->timestamp = other.timestamp;
        this->hist = other.hist;
    }

    ~Message() {}
};

Q_DECLARE_METATYPE(Message)
#endif
