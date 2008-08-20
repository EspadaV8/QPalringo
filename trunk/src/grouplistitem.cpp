//
// C++ Implementation: grouplistitem
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "grouplistitem.h"

GroupListItem::GroupListItem( QWidget *parent, Group *group )
    : ListItem( parent )
{
    
    this->group = group;
    
    this->setIcon( ":/contacts/groupIcon.png" );
    this->setFirstLine( this->group->getName() );
    this->setSecondLine( this->group->getDescription() );
}

QString GroupListItem::getContainerGroup()
{
    return "Group Chat";
}


GroupListItem::~GroupListItem()
{
}

