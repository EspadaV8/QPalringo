/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QPalringo. If not, see <http://www.gnu.org/licenses/>       *
 *                                                                         *
 ***************************************************************************/
#ifndef QPALRINGOCONNECTION_H
#define QPALRINGOCONNECTION_H

#include <QObject>
#include <QMap>
#include "libPalringo/libPalringo.h"
#include "contact.h"
#include "group.h"
#include "message.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class QPalringoConnection : public QObject, public PalringoConnection
{
    Q_OBJECT

    signals:
        int logonSuccessful();
        int logonSuccessful( QString timestamp );
        void messageReceived( QString message,
                              unsigned long long senderID,
                              unsigned long long groupID,
                              QString ct );
        void messageReceived( Message *message );
        void historyMessageReceived( Message *message );
        void gotContactDetails( Contact *contact );
        void gotGroupDetails( Group *group );

    public:
        QPalringoConnection(QString login = "",
                            QString password = "",
                            QString host = "primary.palringo.com",
                            unsigned int port = 443);

        int poll();

        bool sendToContact( QByteArray msg,
                            QString contentType,
                            unsigned long long receiverID );
        bool sendToGroup( QByteArray msg,
                          QString contentType,
                          unsigned long long groupID );
        bool sendMessage( QByteArray msg,
                          QString contentType,
                          unsigned long long groupID,
                          bool isGroup );
        bool getHistoryMessage( qint64 target, bool isGroup, qint32 timestamp );
        void joinGroup( QString groupName );
        void createGroup( QString groupName, QString groupDescription = "", QString groupPassword = "");
        void leaveGroup( quint64 groupID );

    protected:
        virtual int onLogonSuccessfulReceived( headers_t&,
                                               std::string&,
                                               GenericData* );
        virtual int onMesgReceived( headers_t&,
                                     std::string&,
                                     GenericData*);
        virtual int onContactDetailReceived(headers_t& headers,
                                            std::string& body,
                                            GenericData *data);
        virtual int onGroupDetailReceived(headers_t&,
                                          std::string&,
                                          GenericData*);

    private:
        QMap<unsigned long long, Message*> unfinishedMessages;
};

#endif
