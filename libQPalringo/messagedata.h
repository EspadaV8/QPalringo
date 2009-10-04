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
/**
    @author Andrew Smith <espadav8@gmail.com>
*/
class Target;

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
                seconds( other.seconds ),
                useconds( other.useconds ),
                bridgeID( other.bridgeID ),
                hist( other.hist ),
                name( other.name ),
                sender( other.sender ) { }
        ~MessageData() { }

        QString type;
        QByteArray payload;
        quint64 senderID;
        quint64 groupID;
        quint32 seconds;
        quint32 useconds;
        quint32 bridgeID;
        bool hist;
        QString name;
        Target* sender;
};

#endif // MESSAGEDATA_H
