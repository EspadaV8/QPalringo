#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QHash>
#include "bridgetype.h"
#include "services/service.h"
#include "targets/bridgecontact.h"

class Bridge : public Service
{
    Q_OBJECT
    public:
        Bridge();

        void setId( quint32 id );
        void setUsername( QString username );
        void setResource( QString resource );
        void setVendor( QString vendor );
        void setDomain( QString domain );
        void setDisabled( bool disabled );
        void addContact( BridgeContact* contact );

        quint32 getId();
        QString getUsername();
        QString getResource();
        QString getVendor();
        QString getDomain();
        bool isDisabled();
        QHash<quint64, BridgeContact*> getContacts();

    signals:
        void bridgeUpdated();

    private:
        quint32 id;
        QString username;
        QString resource;
        QString vendor;
        QString domain;
        bool disabled;
        QHash<quint64, BridgeContact*> contacts;
};

#endif // BRIDGE_H
