#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include "bridgetype.h"

class Bridge : public QObject
{
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

        quint32 getId();
        qpBridgeType::Type getType();
        QString getNickname();
        QString getUsername();
        QString getResource();
        QString getVendor();
        QString getDomain();
        bool isDisabled();

    private:
        quint32 id;
        qpBridgeType::Type type;
        QString nickname;
        QString username;
        QString resource;
        QString vendor;
        QString domain;
        bool disabled;
};

#endif // BRIDGE_H
