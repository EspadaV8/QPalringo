#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include "bridgetype.h"

class Bridge : public QObject
{
    public:
        Bridge();

        void setNickname( QString nickname );
        void setUsername( QString username );
        void setType( qpBridgeType::Type type );
        void setId( quint32 id );

        QString getNickname();
        QString getUsername();
        qpBridgeType::Type getType();
        quint32 getId();

    private:
        QString nickname;
        QString username;
        qpBridgeType::Type type;
        quint32 id;
};

#endif // BRIDGE_H
