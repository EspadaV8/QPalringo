//
// C++ Implementation: target
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "target.h"

Target::Target(QObject *parent)
 : QObject(parent)
{
}

Target::~Target()
{
}

unsigned long long Target::getID()
{
    return this->id;
}

void Target::setID( unsigned long long id )
{
    this->id = id;
}

QString Target::getTitle()
{
    return "QPalringo";
}

QString Target::getIcon()
{
    return ":/svg/icon.svg";
}
