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
#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <QSharedData>
#include <QString>
#include <QByteArray>
#include <QDateTime>
/**
    @author Andrew Smith <espadav8@gmail.com>
*/

class MessageData : public QSharedData
{
    public:
        MessageData() {}
        MessageData( const MessageData &other )
            : QSharedData( other ),
                type( other.type ),
                payload( other.payload ),
                senderID( other.senderID ),
                groupID( other.groupID ),
                timestamp( other.timestamp ),
                seconds( other.seconds ),
                useconds( other.useconds ),
                hist( other.hist ) { }
        ~MessageData() { }

        QString type;
        QByteArray payload;
        quint64 senderID;
        quint64 groupID;
        QDateTime timestamp;
        bool hist;
};

#endif // MESSAGEDATA_H