#ifndef BRIDGECONTACT_H
#define BRIDGECONTACT_H

#include "target.h"

class BridgeContact : public Target
{
    Q_OBJECT
    public:
        BridgeContact();

        quint32     getBridgeId() const;
        quint64     getContactId() const;
        QString     getName() const;
        QString     getNickname() const;

        void setBridgeId( quint32 bridgeId );
        void setContactId( quint64 contactId );
        void setName( QString name );
        void setNickname( QString nickname );

    private:
        quint32 bridgeId;
        quint64 contactId;
        QString name;
        QString nickname;
};

#endif // BRIDGECONTACT_H
