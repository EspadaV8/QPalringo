#include "bridge.h"

Bridge::Bridge()
{
}

void Bridge::setNickname( QString nickname )
{
    this->nickname = nickname;
}

void Bridge::setUsername( QString username )
{
    this->username = username;
}

void Bridge::setType( qpBridgeType::Type type )
{
    this->type = type;
}

void Bridge::setId( quint32 id )
{
    this->id = id;
}

QString Bridge::getNickname()
{
    return this->nickname;
}

QString Bridge::getUsername()
{
    return this->username;
}

qpBridgeType::Type Bridge::getType()
{
    return this->type;
}

quint32 Bridge::getId()
{
    return this->id;
}
