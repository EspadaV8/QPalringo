#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QHash>
#include "bridgetype.h"
#include "targets/bridgecontact.h"

class Bridge : public QObject
{
    Q_OBJECT
    public:
        Bridge();

        void setId( quint32 id );
        void setType( qpBridgeType::Type type );
        void setNickname( QString nickname );
        void setUsername( QString username );
        void setResource( QString resource );
        void setVendor( QString vendor );
        void setDomain( QString domain );
        void setDisabled( bool disabled );
        void addContact( BridgeContact* contact );
        void setOnlineStatus( qpOnlineStatus::OnlineStatus onlineStatus );

        quint32 getId();
        qpBridgeType::Type getType();
        QString getNickname();
        QString getUsername();
        QString getResource();
        QString getVendor();
        QString getDomain();
        bool isDisabled();
        QHash<quint64, BridgeContact*> getContacts();
        qpOnlineStatus::OnlineStatus getOnlineStatus();

    signals:
        void bridgeUpdated();

    private:
        quint32 id;
        qpBridgeType::Type type;
        QString nickname;
        QString username;
        QString resource;
        QString vendor;
        QString domain;
        bool disabled;
        QHash<quint64, BridgeContact*> contacts;
        qpOnlineStatus::OnlineStatus onlineStatus;
};

#endif // BRIDGE_H
