//
// C++ Interface: connection
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QWidget>
#include "qpalringoconnection.h"

struct Message;
class PalringoWindow;

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class Connection : public QThread
{
    Q_OBJECT
    public:
        Connection(QString  emailAddress = "",
                   QString  password     = "",
                   QString  host         = "primary.palringo.com",
                   int      port         = 443);
        ~Connection();

        void run();
        void sendMessage( unsigned long long receiverID, bool isGroup, Message *message );
        void getHistoryMessage( qint64 target, bool isGroup, qint32 timestamp );

    private:
        QString emailAddress;
        QString password;
        QString host;
        int     port;

        QPalringoConnection *conn;
};

#endif
