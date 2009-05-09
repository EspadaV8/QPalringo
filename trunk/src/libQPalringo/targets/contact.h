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
#ifndef CONTACT_H
#define CONTACT_H

#include "target.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class Contact : public Target
{
    Q_OBJECT
    public:
        Contact(QObject *parent = 0);
        ~Contact();

        QString     getNickname() const;
        QString     getStatusline() const;
        quint64     getGroup() const;
        int         getOnlineStatus() const;
        int         getBridgeType() const;
        int         getDeviceType() const;
        bool        getIsContact() const;
        virtual QString getTitle() const;

        void        setNickname( QString );
        void        setStatusline( QString );
        void        setGroup( quint64 );
        void        setOnlineStatus( int );
        void        setBridgeType( int );
        void        setDeviceType( int );
        void        setIsContact( bool );

    signals:
        void        updateNickname();
        void        updateStatusline();
        void        updateOnlineStatus();

    protected:
        QString     nickname;
        QString     statusline;
        quint64     group;
        int         onlineStatus;
        int         bridgeType;
        int         deviceType;
        bool        isContact;

};

#endif
