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

        void setType( QString type );
        void setOnlineStatus( QString onlineStatus );
        void setNickname( QString nickname );
        void setStatus( QString status );

        QString getType() const;
        QString getOnlineStatus() const;
        QString getNickname() const;
        QString getStatus() const;
        virtual QString getIcon() const;

    protected:
        QString type;
        QString onlineStatus;
        QString nickname;
        QString status;
        
    signals:
        virtual void serviceUpdated();
        
    protected slots:
        virtual void loggedOn();
        virtual void loginService();

    public slots:
        virtual void changeOnlineStatus();
};

#endif
