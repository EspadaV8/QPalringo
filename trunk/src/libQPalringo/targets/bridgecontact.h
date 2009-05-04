#ifndef BRIDGECONTACT_H
#define BRIDGECONTACT_H

#include "contact.h"
#include "../onlinestatus.h"

class BridgeContact : public Contact
{
    Q_OBJECT
    public:
        BridgeContact();

        quint32     getBridgeId() const;
        QString     getName() const;
        QString     getCurrentMedia() const;

        void setBridgeId( quint32 bridgeId );
        void setName( QString name );
        void setCurrentMedia( QString currentMedia );

    private:
        quint32 bridgeId;
        QString name;
        QString currentMedia;

    signals:
        void nameChanged();
        void currentMediaChanged();
};

#endif // BRIDGECONTACT_H
