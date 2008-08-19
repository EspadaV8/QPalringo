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
#include "palringowindow.h"
#include "contact.h"
#include "group.h"
#include "messageitem.h"
#include "chatwindow.h"
#include "connection.h"

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

        void openChatWindow( Contact *contact );
        void removeChatWindow( Contact *contact );

        void openPalringoConnection( QString email, QString password );

        void sendMessage( Contact *contact, Message *message );

        void addNewContact( Contact *contact );
        QHash<unsigned long long, Contact*> getContacts();

        void addGroup( Group *group );

    public slots:
        void updateContacts();
        void messageReceived( QString message, unsigned long long senderID, unsigned long long group, QString contentType );

    signals:
        void newGroupAdded( Group *group );
        void newContact( Contact *contact );

        void userContactReceived( Contact *contact );
        void contactDetailReceived( Contact *contact );

    private:
        PalringoWindow *mainWindow ;
        Connection *connection;

        bool checkChatWindowOpen( Contact *contact );

        QHash<Contact *, ChatWindow *> openWindows;

        // TODO: We need a map of contacts and messages that haven't been read yet
        QMultiMap<Contact*, Message*> unreadMessages;

        QHash<unsigned long long, Contact* > contacts;
        QHash<unsigned long long, Group* > groups;
};

#endif
