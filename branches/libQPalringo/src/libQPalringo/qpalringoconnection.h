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

#include <QThread>
#include <QMap>
#include <QReadWriteLock>
#include <QString>
#include <QVariant>
#include "../libPalringo/libPalringo.h"
#include "targets/target.h"
#include "targets/contact.h"
#include "targets/group.h"
#include "message.h"
#include "user.h"
#include "headers.h"
#include "qpheaderattributes.h"
#include "qpcommands.h"
#include "errorcodes.h"
#include "qpgenericdata.h"

/**
    @author Andrew Smith <espadav8@gmail.com>
*/
class QPalringoConnection : public QThread, public PalringoConnection
{
    Q_OBJECT
    signals:
        int logonSuccessful();
        int logonSuccessful( QString timestamp );
        void messageReceived( QString message, quint64 senderID, quint64 groupID, QString ct );
        void messageReceived( Message message );
        void historyMessageReceived( Message message );
        void gotContactDetails( Contact *contact );
        void gotGroupDetails( Group *group );

    public:
        QPalringoConnection(QString login = "",
                            QString password = "",
                            QString clientType = "Linux",
                            QString host = "primary.palringo.com",
                            unsigned int port = 443 );

        void run();

        bool sendMessage( Target* target, Message message );
        void getMesgHist( Target* target, QString timestampStr, qint32 count );
        bool sendCmd( QString command, Headers headers, QByteArray body = "" );
        void joinGroup( QString groupName );
        void createGroup( QString groupName, QString groupDescription = "", QString groupPassword = "");
        void leaveGroup( quint64 groupID );
        bool updateContactDetail( QString detail, QString value );

        User getUser();
        Group* getGroup( quint64 groupID );
        Contact* getContact( quint64 contactID );

        QHash<quint64, Contact*> getAllContacts();
        QHash<quint64, Contact*> getContactListContacts();
        QHash<quint64, Contact*> getGroupContacts( quint64 groupID );

    protected:
        virtual int onLogonSuccessfulReceived( headers_t&, std::string&, GenericData* );
        virtual int onMesgReceived( headers_t&, std::string&, GenericData*);
        virtual int onContactDetailReceived(headers_t& headers, std::string& body, GenericData *data);
        virtual int onGroupDetailReceived(headers_t&, std::string&, GenericData*);
        virtual int onSubProfileReceived(headers_t& headers, std::string& body, GenericData *data );

    protected slots:
        int onPingSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onLogonSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onByeSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onAuthSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onContactUpdateSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onContactAddRespSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onGroupSubscribeSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onGroupUnsubSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onGroupCreateSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onGroupInviteSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onGroupAdminSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onMesgSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onMesgStoredSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onMesgHistSent( Headers& headers, QByteArray& body, qpGenericData *data );
        int onRegSent( Headers& headers, QByteArray& body, qpGenericData *data );

    private:
        QMap<QString, QString> outSignals;
        QMap<QString, QString> inSignals;

        void initOutSignals();
        void initInSignals();

        QMap<quint64, Message> unfinishedMessages;
        QHash<quint64, Contact* > contacts;
        QHash<quint64, Group* > groups;

        QReadWriteLock contactLock;

        qint64 timestampDifference;
        User user;
        QString serverTimestamp;

    signals:
        void pingSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void logonSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void byeSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void authSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void contactUpdateSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void contactAddRespSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void groupSubscribeSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void groupUnsubSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void groupCreateSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void groupInviteSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void groupAdminSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void mesgSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void mesgStoredSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void mesgHistSent( Headers& headers, QByteArray& body, qpGenericData* data );
        void regSent( Headers& headers, QByteArray& body, qpGenericData* data );
};
#endif
