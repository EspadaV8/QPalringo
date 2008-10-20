//
// C++ Interface: contact
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

        QString     getNickname();
        QString     getStatusline();
        unsigned long long    getGroup();
        int         getOnlineStatus();
        int         getBridgeType();
        int         getDeviceType();
        bool        getIsContact();
        virtual QString getTitle();
        virtual QString getIcon();

        void        setNickname( QString );
        void        setStatusline( QString );
        void        setGroup( unsigned long long );
        void        setOnlineStatus( int );
        void        setBridgeType( int );
        void        setDeviceType( int );
        void        setIsContact( bool );

    signals:
        void        updateNickname( QString );
        void        updateStatusline( QString );
        void        updateOnlineStatus( int );

    private:
        QString     nickname;
        QString     statusline;
        unsigned long long    group;
        int         onlineStatus;
        int         bridgeType;
        int         deviceType;
        bool        isContact;

};

#endif
