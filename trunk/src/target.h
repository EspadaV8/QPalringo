//
// C++ Interface: target
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TARGET_H
#define TARGET_H

#include <QObject>

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class Target : public QObject
{
    Q_OBJECT
    public:
        Target(QObject *parent = 0);
        ~Target();

    protected:
        unsigned long long id;

};

#endif
