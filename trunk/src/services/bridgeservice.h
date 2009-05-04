#ifndef BRIDGESERVICE_H
#define BRIDGESERVICE_H

#include "service.h"

class BridgeService : public Service
{
    public:
        BridgeService();
        BridgeService( quint32 bridgeId );
        QString getIcon() const;
        void loginService();

        void setBridgeId( quint32 bridgeId );
        quint32 getBridgeId();

    private:
        quint32 bridgeId;
};

#endif // BRIDGESERVICE_H
