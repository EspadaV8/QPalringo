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

#include <QObject>
#include <QSet>

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class Group : public QObject
{
    Q_OBJECT
    public:
        Group(QObject *parent = 0);
        ~Group();
        
        QString         getName();
        QString         getDescription();
        unsigned long long        getID();
        QSet<unsigned long long>  getContacts();

        void            setName( QString name );
        void            setDescription( QString description );
        void            setID( unsigned long long );
        void            setContacts( QSet<unsigned long long> );

    private:
        unsigned long long        id;
        QString         name;
        QString         description;
        QSet<unsigned long long>  contacts;
};

#endif
