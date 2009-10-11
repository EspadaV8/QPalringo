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
#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include "bridgetype.h"
#include "onlinestatus.h"

/**
        @author Andrew Smith <espadav8@gmail.com>
*/
class Service : public QObject
{
    Q_OBJECT
    public:
        Service(QObject *parent = 0);
        ~Service();

        void goOnline();

        void setType( qpBridgeType::Type type );
        void setOnlineStatus( qpOnlineStatus::OnlineStatus onlineStatus );
        void setNickname( QString nickname );
        void setStatus( QString status );
        void setId( quint32 id );

        qpBridgeType::Type getType() const;
        virtual qpOnlineStatus::OnlineStatus getOnlineStatus() const;
        virtual QString getNickname() const;
        virtual QString getStatus() const;
        virtual QString getIcon() const;
        virtual quint32 getId() const;

    protected:
        qpBridgeType::Type type;
        qpOnlineStatus::OnlineStatus onlineStatus;
        QString nickname;
        QString status;
        quint32 id;

    signals:
        virtual void serviceUpdated();

    protected slots:
        virtual void loggedOn();
        virtual void loginService();

    public slots:
        virtual void changeOnlineStatus();
};

#endif
