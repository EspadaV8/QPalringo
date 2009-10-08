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
#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QHash>
#include <QMultiMap>
#include <QDateTime>

#include "targets/target.h"
#include "qpalringoconnection.h"

class Tools;
extern Tools *tools_;

/**
        @author Andrew Smith <espadav8@gmail.com>
*/
class Tools : public QObject
{
    Q_OBJECT
    public:
        Tools( QObject *parent = 0 );
        ~Tools();

        void sendMessage( Target *target, Message message );
        void getHistoryMessage( Target *target, QString timestamp );

        void HeaderWrite( QByteArray* data, bool stereo, short bitsPerSample, int numberOfSamples, int sampleRate );

        QDateTime convertTimestampToQDateTime( QString timestamp = "0", bool receivedMessage = false );
        quint32 getTimestampDifference();

        // void showContactProperties( Contact *contact );

        void joinGroup( QString groupName );
        void createGroup( QString groupName );
        void leaveGroup( quint64 groupID );
        void setBridgeStatus( quint32 bridgeId, qpOnlineStatus::OnlineStatus onlineStatus );

        bool isLoggedIn();

        void playSound( QString filename );

        Contact* getContact( quint64 contactID );
        QHash<quint64, Contact*> getContactListContacts();
        QHash<quint64, Contact*> getGroupContacts( quint64 groupID );
        Group* getGroup( quint64 groupID );
        Bridge* getBridge( quint32 bridgeID );
        BridgeContact* getBridgeContact( quint32 bridgeID, quint64 contactID );

        static QString getTargetIcon( Target* target );
        static User getUser();

    public slots:
        void logonSuccessful();
        void logonSuccessful( QString timestamp );
        void addContact( Contact *contact );
        void messageReceived( Message message );
        void disconnected();
        bool updateContactDetail( QString detail, QString value );
        void openPalringoConnection( QString email = "", QString password = "" );

    signals:
        void connected();
        void cleanUp();
        void newGroupAdded( Group *group );
        void newContact( Contact *contact );
        void gotServiceDetails( Service* service );
        void gotBridgeContact( BridgeContact* contact );

        void userContactReceived( Contact *contact );
        void contactDetailReceived( Contact *contact );
        void groupLeft( quint64 groupID );

        void newPendingMessage( Target* target );
        void historyMessageReceived( Message message );

    private:
        bool loggedIn;
        static QPalringoConnection *connection;

        //QHash<Target*, ChatWindow *> openWindows;

        // TODO: We need a map of contacts and messages that haven't been read yet
        QMultiMap<Contact*, Message> unreadMessages;

        // history tracking
        bool gettingHistory;
        Target* historyTarget;

        // The timestamp of the server
        qint32 timestampDifference;
        void calcServerTimestampDifference( QString timestamp );
};

#endif
