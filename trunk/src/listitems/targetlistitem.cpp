#include "targetlistitem.h"

TargetListItem::TargetListItem( QWidget *parent, Target* target )
    : ListItem( parent )
{
    this->target = target;
}

Target* TargetListItem::getTarget()
{
    return this->target;
}
