#ifndef BRIDGESERVICEPANE_H
#define BRIDGESERVICEPANE_H

#include "servicepane.h"
#include "services/bridge.h"
#include "targets/bridgecontact.h"

class BridgeServicePane : public ServicePane
{
    Q_OBJECT
    public:
        BridgeServicePane( Bridge* bridge = 0, QWidget* parent = 0 );

    public slots:
        virtual void updateContacts();

    private:
        Bridge* bridge;
};

#endif // BRIDGESERVICEPANE_H
