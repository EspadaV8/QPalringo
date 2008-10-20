//
// C++ Interface: group
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GROUP_H
#define GROUP_H

#include "target.h"
#include <QSet>

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class Group : public Target
{
    Q_OBJECT
    public:
        Group(QObject *parent = 0);
        ~Group();

        QString                     getName();
        QString                     getDescription();
        QSet<unsigned long long>    getContacts();
        bool                        hasContact( unsigned long long id );
        virtual QString getTitle();
        virtual QString getIcon();

        void            setName( QString name );
        void            setDescription( QString description );
        void            setContacts( QSet<unsigned long long> );

    private:
        QString         name;
        QString         description;
        QSet<unsigned long long>  contacts;
};

#endif
