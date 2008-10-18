//
// C++ Interface: service
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
        
        QString Type;
        QString OnlineStatus;
        QString Nickname;
        QString Status;
        QString Group;
        
        void goOnline();
        
    signals:
        virtual void serviceUpdated();
        
    private slots:
        virtual void loggedOn();
};

#endif
