#ifndef TARGETLISTITEM_H
#define TARGETLISTITEM_H

#include "listitem.h"
#include "libQPalringo/targets/target.h"

class TargetListItem : public ListItem
{
    Q_OBJECT
    public:
        TargetListItem( QWidget* parent = 0, Target* target = NULL );
        Target* getTarget();

    protected:
        Target* target;
};

#endif // TARGETLISTITEM_H
