#ifndef BRIDGESERVICE_H
#define BRIDGESERVICE_H

#include "service.h"

class BridgeService : public Service
{
    public:
        BridgeService();
        QString getIcon() const;
};

#endif // BRIDGESERVICE_H
