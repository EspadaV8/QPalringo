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
        QSet<quint64>    getContacts();
        bool                        hasContact( quint64 id );
        virtual QString getTitle();
        virtual QString getIcon();

        void            setName( QString name );
        void            setDescription( QString description );
        void            setContacts( QSet<quint64> );

    private:
        QString         name;
        QString         description;
        QSet<quint64>  contacts;
};

#endif
