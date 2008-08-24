//
// C++ Interface: qpalringoconnection
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
        void messageReceived( QString message,
                              unsigned long long senderID,
                              unsigned long long groupID,
                              QString ct );
        void messageReceived( Message *message );
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
