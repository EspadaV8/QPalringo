#include "targetlistitem.h"

TargetListItem::TargetListItem( Target* target, QWidget *parent )
    : ListItem( parent )
{
    this->target = target;
}

Target* TargetListItem::getTarget()
{
    return this->target;
}
