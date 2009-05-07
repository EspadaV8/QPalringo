#ifndef BRIDGECONTACTLISTITEM_H
#define BRIDGECONTACTLISTITEM_H

#include "targetlistitem.h"
#include "libQPalringo/targets/bridgecontact.h"

class BridgeContactListItem : public TargetListItem
{
    Q_OBJECT
    public:
        BridgeContactListItem( QWidget *parent, BridgeContact *contact );
        QString getContainerGroup();

    signals:
        void containerGroupChanged( ListItem* );

    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );

    private:
        BridgeContact *contact;
        QString getIcon();

    private slots:
        void startChat();
        void updateDetails();
        void updateOnlineStatus();
};

#endif // BRIDGECONTACTLISTITEM_H
