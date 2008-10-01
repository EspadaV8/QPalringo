//
// C++ Interface: message
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QByteArray>
#include <QDateTime>
/**
	@author Andrew Smith <espadav8@gmail.com>
*/
struct Message
{
    QString type;
    QByteArray payload;
    unsigned long long senderID;
    unsigned long long groupID;
    QDateTime timestamp;
    bool hist;
};

#endif
