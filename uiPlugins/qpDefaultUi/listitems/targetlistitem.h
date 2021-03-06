#ifndef TARGETLISTITEM_H
#define TARGETLISTITEM_H

#include "listitem.h"
#include "targets/target.h"

class TargetListItem : public ListItem
{
    Q_OBJECT
    public:
        TargetListItem( Target* target = NULL, QWidget* parent = 0 );
        Target* getTarget();

    protected:
        Target* target;

    signals:
        void focusChatWindow( Target* target );
};

#endif // TARGETLISTITEM_H
