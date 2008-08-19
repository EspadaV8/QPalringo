//
// C++ Interface: grouplistitem
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GROUPLISTITEM_H
#define GROUPLISTITEM_H

#include <QWidget>
#include "listitem.h"
#include "group.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class GroupListItem : public ListItem
{
    Q_OBJECT
    public:
        GroupListItem( QWidget *parent = 0, Group *group = NULL );
        ~GroupListItem();
        
        virtual QString getContainerGroup();
        
    private:
        Group *group;

};

#endif
