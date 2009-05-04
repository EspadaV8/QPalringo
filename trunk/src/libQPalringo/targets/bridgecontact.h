#ifndef BRIDGECONTACT_H
#define BRIDGECONTACT_H

#include "target.h"
#include "../onlinestatus.h"

class BridgeContact : public Target
{
    Q_OBJECT
    public:
        BridgeContact();

        quint32     getBridgeId() const;
        quint64     getContactId() const;
        qpOnlineStatus::OnlineStatus getOnlineStatus() const;
        QString     getName() const;
        QString     getNickname() const;
        QString     getStatus() const;
        QString     getCurrentMedia() const;

        void setBridgeId( quint32 bridgeId );
        void setContactId( quint64 contactId );
        void setOnlineStatus( qpOnlineStatus::OnlineStatus onlineStatus );
        void setName( QString name );
        void setNickname( QString nickname );
        void setStatus( QString status );
        void setCurrentMedia( QString currentMedia );

    private:
        quint32 bridgeId;
        quint64 contactId;
        qpOnlineStatus::OnlineStatus onlineStatus;
        QString name;
        QString nickname;
        QString status;
        QString currentMedia;
};

#endif // BRIDGECONTACT_H
