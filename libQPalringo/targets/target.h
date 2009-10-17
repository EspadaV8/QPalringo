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
#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include "../message.h"

/**
        @author Andrew Smith <espadav8@gmail.com>
*/
class Message;

class Target : public QObject
{
    Q_OBJECT
    public:
        Target(QObject *parent = 0);
        ~Target();

        static const int CONTACT = 1;
        static const int GROUP = 2;
        static const int BRIDGECONTACT = 3;

        quint64 getID() const;
        void setID( quint64 );
        int getType();

        virtual QString getTitle() const;

        virtual void addMessage( Message message );
        void clearPending();
        QList<Message> getPendingMessages();

    signals:
        void pendingMessage();
        void pendingMessage( Target* target );
        void insertMessage();
        void clearedPendingMessages();

    protected:
        quint64 id;
        int type;
        QList<Message> pendingMessages;

};

#endif
