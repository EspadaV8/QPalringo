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

#include <QSharedData>
#include <QString>

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
                hist( other.hist ) { }
        ~MessageData() { }

        QString type;
        QByteArray payload;
        quint64 senderID;
        quint64 groupID;
        QDateTime timestamp;
        bool hist;
};

class Message
{
    public:
        Message()
        {
            d = new MessageData;
            qRegisterMetaType<Message>("Message");
        }

        Message( QString type, QByteArray payload,
                 quint64 senderID, quint64 groupID,
                 QDateTime timestamp, bool hist )
        {
            d = new MessageData;
            qRegisterMetaType<Message>("Message");
            qDebug( "New setter" );

            setType( type );
            setPayload( payload );
            setSenderID( senderID );
            setGroupID( groupID );
            setTimestamp( timestamp );
            setHist( hist );
        }

        Message( const Message &other ) : d ( other.d ) {}
        ~Message() {}

        void setType( QString type )
        {
            d->type = type;
        }
        void setPayload( QByteArray payload )
        {
            d->payload = payload;
        }
        void setSenderID( quint64 senderID )
        {
            d->senderID = senderID;
        }
        void setGroupID( quint64 groupID )
        {
            d->groupID = groupID;
        }
        void setTimestamp( QDateTime timestamp )
        {
            d->timestamp = timestamp;
        }
        void setHist( bool hist )
        {
            d->hist = hist;
        }

        QString type() const
        {
            return d->type;
        }
        QByteArray payload() const
        {
            return d->payload;
        }
        quint64 senderID() const
        {
            return d->senderID;
        }
        quint64 groupID() const
        {
            return d->groupID;
        }
        QDateTime timestamp() const
        {
            return d->timestamp;
        }
        bool hist() const
        {
            return d->hist;
        }

    private:
        QSharedDataPointer<MessageData> d;
};

Q_DECLARE_METATYPE(Message)
#endif
