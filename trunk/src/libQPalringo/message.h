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
#include <QSharedData>
#include <QString>
#include <QByteArray>
#include "messagedata.h"
/**
    @author Andrew Smith <espadav8@gmail.com>
*/

class Message
{
    public:
        Message();
        Message( QString type, QByteArray payload, quint64 senderID, quint64 groupID,
                    quint32 seconds, quint32 useconds, quint32 bridgeID, bool hist );
        Message( const Message &other );
        ~Message();

        void setType( QString type );
        void setPayload( QByteArray payload );
        void setSenderID( quint64 senderID );
        void setGroupID( quint64 groupID );
        void setSeconds( quint32 seconds );
        void setUseconds( quint32 useconds );
        void setBridgeID( quint32 bridgeID );
        void setHist( bool hist );

        QString type() const;
        QByteArray payload() const;
        quint64 senderID() const;
        quint64 groupID() const;
        quint32 seconds() const;
        quint32 useconds() const;
        quint32 bridgeID() const;
        bool hist() const;

        bool operator< ( const Message &other );
        bool operator<= ( const Message &other );
        bool operator> ( const Message &other );
        bool operator>= ( const Message &other );
        bool operator== ( const Message &other );

    private:
        QSharedDataPointer<MessageData> d;
};

Q_DECLARE_METATYPE(Message)
#endif
