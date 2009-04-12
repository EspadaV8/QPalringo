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
#include <QReadWriteLock>
#include <QDateTime>

#include "libQPalringo/targets/target.h"
#include "libQPalringo/qpalringoconnection.h"

#include "palringowindow.h"
#include "messageitem.h"
#include "chatwindow.h"

class Tools;
extern Tools *tools_;

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class Tools : public QObject
{
    Q_OBJECT
    public:
        Tools( PalringoWindow *parent = 0 );
        ~Tools();

        bool checkChatWindowOpen( Target *target );
        void openChatWindow( Target *target );
        void removeChatWindow( Target *target );

        void openPalringoConnection( QString email = "", QString password = "");

        void sendMessage( Target *target, Message message );
        void getHistoryMessage( Target *target, QString timestamp );

        Contact* getContact( quint64 contactID );
        QHash<quint64, Contact*> getContacts();
        QHash<quint64, Contact*> getContacts( quint64 groupID = 0 );

        QString formatMessageText( QByteArray messagePayload );
        QString tagURLs( QString text );

        void HeaderWrite( QByteArray* data, bool stereo, short bitsPerSample, int numberOfSamples, int sampleRate );

        QDateTime convertTimestampToQDateTime( QString timestamp = "0", bool receivedMessage = false );
        QDateTime getMessageTimestamp( Message message );
        quint32 getTimestampDifference();
        
        QPixmap getPixmap( QString iconFilename );
        
        void showContactProperties( Contact *contact );
        
        void joinGroup( QString groupName );
        void createGroup( QString groupName );
        void leaveGroup( quint64 groupID );

        bool isLoggedIn();

        void playSound( QString filename );

        User getUser();

    public slots:
        void logonSuccessful();
        void logonSuccessful( QString timestamp );
        void addContact( Contact *contact );
        void messageReceived( Message message );
        void historyMessageReceived( Message message );
        void disconnected();
        bool updateContactDetail( QString detail, QString value );

    signals:
        void connected();
        void newGroupAdded( Group *group );
        void newContact( Contact *contact );

        void userContactReceived( Contact *contact );
        void contactDetailReceived( Contact *contact );
        void groupLeft( quint64 groupID );

        void newPendingMessage( Target* target );

    private:
        bool loggedIn;
        PalringoWindow *mainWindow ;
        QPalringoConnection *connection;

        QHash<Target*, ChatWindow *> openWindows;

        // TODO: We need a map of contacts and messages that haven't been read yet
        QMultiMap<Contact*, Message> unreadMessages;

        QHash<quint64, Contact* > contacts;

        QReadWriteLock contactLock;

        // history tracking
        bool gettingHistory;
        Target* historyTarget;

        // The timestamp of the server
        qint32 timestampDifference;
        void calcServerTimestampDifference( QString timestamp );
};

#endif
