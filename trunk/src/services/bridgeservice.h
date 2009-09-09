#ifndef BRIDGESERVICE_H
#define BRIDGESERVICE_H

#include "service.h"
#include "bridge.h"

class BridgeService : public Service
{
    public:
        BridgeService();
        BridgeService( Bridge* bridge );

        virtual qpOnlineStatus::OnlineStatus getOnlineStatus() const;
        virtual QString getNickname() const;
        virtual QString getStatus() const;
        virtual QString getIcon() const;

        void loginService();

        void setBridgeId( quint32 bridgeId );
        quint32 getBridgeId();

    private:
        Bridge* bridge;
};

#endif // BRIDGESERVICE_H
