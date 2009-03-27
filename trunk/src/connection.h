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
        void sendMessage( Target* target, Message message );
        void getHistoryMessage( qint64 target, bool isGroup, qint32 timestamp );
        
        void joinGroup( QString groupName );
        void createGroup( QString groupName );
        void leaveGroup( quint64 groupID );

    private:
        QString emailAddress;
        QString password;
        QString host;
        int     port;

        QPalringoConnection *conn;
    
    signals:
        void disconnected();
};

#endif
