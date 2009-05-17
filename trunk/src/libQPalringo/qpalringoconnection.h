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
#ifndef QPALRINGOCONNECTION_H
#define QPALRINGOCONNECTION_H

#include <QtNetwork>
#include <QQueue>
#include <QMap>
#include <QReadWriteLock>
#include <QString>
#include <QVariant>
#include "targets/target.h"
#include "targets/contact.h"
#include "targets/group.h"
#include "targets/bridgecontact.h"
#include "bridge.h"
#include "message.h"
#include "user.h"
#include "headers.h"
#include "qpheaderattributes.h"
#include "qpcommands.h"
#include "errorcodes.h"
#include "qpgenericdata.h"

struct IncomingCommand
{
    Headers headers;
    QString command;
    QByteArray body;
    bool complete;
};

/**
    @author Andrew Smith <espadav8@gmail.com>
*/
class QPalringoConnection : public QObject
{
    Q_OBJECT
    public:
        QPalringoConnection(QString login = "",
                            QString password = "",
                            QString clientType = "Linux",
                            QString host = "primary.palringo.com",
                            quint16 port = 443,
                            qint32 protocol = 1 );

        virtual int connectClient( bool reconnect = false );
        void setProxy( QNetworkProxy proxy );

        bool sendMessage( Target* target, Message message );
        void getMesgHist( Target* target, QString timestampStr, qint32 count );
        bool sendCmd( QString command, Headers headers, QByteArray body = "" );
        void joinGroup( QString groupName );
        void createGroup( QString groupName, QString groupDescription = "", QString groupPassword = "");
        void leaveGroup( quint64 groupID );
        bool updateContactDetail( QString detail, QString value );
        void setBridgeStatus( quint32 bridgeId, qpOnlineStatus::OnlineStatus onlineStatus );

        User getUser();
        Group* getGroup( quint64 groupID );
        Contact* getContact( quint64 contactID );
        Bridge* getBridge( quint32 bridgeID );
        BridgeContact* getBridgeContact( quint32 bridgeID, quint64 contactID );

        QHash<quint64, Contact*> getAllContacts();
        QHash<quint64, Contact*> getContactListContacts();
        QHash<quint64, Contact*> getGroupContacts( quint64 groupID );

    protected slots:
        /** out going message slots **/
        int onPingSent( Headers& headers, QByteArray& body );
        int onLogonSent( Headers& headers, QByteArray& body );
        int onByeSent( Headers& headers, QByteArray& body );
        int onAuthSent( Headers& headers, QByteArray& body );
        int onContactUpdateSent( Headers& headers, QByteArray& body );
        int onContactAddRespSent( Headers& headers, QByteArray& body );
        int onGroupSubscribeSent( Headers& headers, QByteArray& body );
        int onGroupUnsubSent( Headers& headers, QByteArray& body );
        int onGroupCreateSent( Headers& headers, QByteArray& body );
        int onGroupInviteSent( Headers& headers, QByteArray& body );
        int onGroupAdminSent( Headers& headers, QByteArray& body );
        int onMesgSent( Headers& headers, QByteArray& body );
        int onMesgStoredSent( Headers& headers, QByteArray& body );
        int onMesgHistSent( Headers& headers, QByteArray& body );
        int onRegSent( Headers& headers, QByteArray& body );

        /** incoming message slots **/
        void onAuthRecieved( const Headers& headers, const QByteArray& body );
        void onLogonSuccessfulReceived( const Headers& headers, const QByteArray& body );
        void onMesgReceived( const Headers& headers, const QByteArray& body );
        void onContactDetailReceived( const Headers& headers, const QByteArray& body );
        void onGroupDetailReceived( const Headers& headers, const QByteArray& body );
        void onSubProfileReceived( const Headers& headers, const QByteArray& body );
        void onPingReceived( const Headers& headers, const QByteArray& body );
        void onResponseReceived( const Headers& headers, const QByteArray& body );
        void onBridgeReceived( const Headers& headers, const QByteArray& body );
        void onBridgeContactReceived( const Headers& headers, const QByteArray& body );
        void onBridgeOnReceived( const Headers& headers, const QByteArray& body );

    private:
        QTcpSocket* socket;
        QByteArray inBuffer;
        QByteArray outBuffer;
        quint64 messageId;

        QQueue<IncomingCommand> incomingCommands;

        QString clientType;
        QString host;
        quint16 port;

        QMap<QString, QString> outSignals;
        QMap<QString, QString> inSignals;

        QMap<quint64, Message> unfinishedMessages;
        QHash<quint64, Contact* > contacts;
        QHash<quint64, Group* > groups;
        QHash<quint64, Bridge* > bridges;

        QReadWriteLock contactLock;

        qint64 timestampDifference;
        User user;
        QString serverTimestamp;

        void initOutSignals();
        void initInSignals();
        int parseCmd( const QByteArray& data );

        /** taken from libPalringo **/
        qint32 wordSize_;
        quint32 receivedData_;
        qint32 protocolVersion_;
        qint32 compression_;
        quint64 packetSeq_;
        quint32 outMessageCount_;
        qint32 mesg_id_;
        QString RK_;
        bool encryption_;

    private slots:
        void socketError( QAbstractSocket::SocketError socketError );
        void pollRead();

    signals:
        void disconnected();
        int logonSuccessful();
        int logonSuccessful( QString timestamp );
        void messageReceived( QString message, quint64 senderID, quint64 groupID, QString ct );
        void messageReceived( Message message );
        void historyMessageReceived( Message message );
        void gotContactDetails( Contact *contact );
        void gotGroupDetails( Group *group );
        void gotBridgeDetails( Bridge *bridge );
        void gotBridgeContact( BridgeContact *contact );

        /** out going message signals **/
        void pingSent( Headers& headers, QByteArray& body );
        void logonSent( Headers& headers, QByteArray& body );
        void byeSent( Headers& headers, QByteArray& body );
        void authSent( Headers& headers, QByteArray& body );
        void contactUpdateSent( Headers& headers, QByteArray& body );
        void contactAddRespSent( Headers& headers, QByteArray& body );
        void groupSubscribeSent( Headers& headers, QByteArray& body );
        void groupUnsubSent( Headers& headers, QByteArray& body );
        void groupCreateSent( Headers& headers, QByteArray& body );
        void groupInviteSent( Headers& headers, QByteArray& body );
        void groupAdminSent( Headers& headers, QByteArray& body );
        void mesgSent( Headers& headers, QByteArray& body );
        void mesgStoredSent( Headers& headers, QByteArray& body );
        void mesgHistSent( Headers& headers, QByteArray& body );
        void regSent( Headers& headers, QByteArray& body );

        /** incoming message signals **/
        void authRecieved( const Headers& headers, const QByteArray& body );
        void logonSuccessfulRecieved( const Headers& headers, const QByteArray& body );
        void logonFailedRecieved( const Headers& headers, const QByteArray& body );
        void contactDetailRecieved( const Headers& headers, const QByteArray& body );
        void groupDetailRecieved( const Headers& headers, const QByteArray& body );
        void mesgRecieved( const Headers& headers, const QByteArray& body );
        void pingRecieved( const Headers& headers, const QByteArray& body );
        void responseRecieved( const Headers& headers, const QByteArray& body );
        void bridgeRecieved( const Headers& headers, const QByteArray& body );
        void bridgeContactRecieved( const Headers& headers, const QByteArray& body );
        void bridgeMesgRecieved( const Headers& headers, const QByteArray& body );
        void bridgeOnRecieved( const Headers& headers, const QByteArray& body );
};
#endif
