//
// C++ Interface: tools
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QHash>
#include <QMultiMap>
#include <QReadWriteLock>
#include <QDateTime>
#include "palringowindow.h"
#include "target.h"
#include "messageitem.h"
#include "chatwindow.h"
#include "connection.h"

struct User
{
    unsigned long long userID;
    QString nickname;
    QString status;
    QString lastOnline;
};

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

        void openChatWindow( Target *target, bool isGroup = false );
        void removeChatWindow( Target *target );

        void openChatWindow( Contact *contact );
        void openChatWindow( Group *group );

        void openPalringoConnection( QString email, QString password );

        void sendMessage( Target *target, bool isGroup, Message *message );
        void getHistoryMessage( Target *target, bool isGroup, QDateTime timestamp );

        Contact* getContact( unsigned long long contactID );
        QHash<unsigned long long, Contact*> getContacts();
        QHash<unsigned long long, Contact*> getContacts( unsigned long long groupID = 0 );

        User *user;
        void setUser( unsigned long long userID, QString nickname, QString status, QString lastOnline );

        QString formatMessageText( QByteArray messagePayload );
        QString tagURLs( QString text );

        void HeaderWrite( QByteArray* data, bool stereo, short bitsPerSample, int numberOfSamples, int sampleRate );

        QDateTime convertTimestampToQDateTime( QString timestamp );
        quint32 getTimestampDifference();

    public slots:
        void logonSuccessful();
        void logonSuccessful( QString timestamp );
        void addGroup( Group *group );
        void addContact( Contact *contact );
        void messageReceived( QString message, unsigned long long senderID, unsigned long long group, QString contentType );
        void messageReceived( Message* message );
        void historyMessageReceived( Message* message );

    signals:
        void connected();
        void newGroupAdded( Group *group );
        void newContact( Contact *contact );

        void userContactReceived( Contact *contact );
        void contactDetailReceived( Contact *contact );

    private:
        bool loggedIn;
        PalringoWindow *mainWindow ;
        Connection *connection;

        bool checkChatWindowOpen( Target *target );

        QHash<Target*, ChatWindow *> openWindows;

        // TODO: We need a map of contacts and messages that haven't been read yet
        QMultiMap<Contact*, Message*> unreadMessages;

        QHash<unsigned long long, Contact* > contacts;
        QHash<unsigned long long, Group* > groups;

        QReadWriteLock contactLock;

        // history tracking
        bool gettingHistory;
        Target* historyTarget;
        bool historyTargetIsGroup;

        // The timestamp of the server
        qint32 timestampDifference;
        void calcServerTimestampDifference( QString timestamp );
};

#endif
