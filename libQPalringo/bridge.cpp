#include "bridge.h"

Bridge::Bridge()
{
}


void Bridge::setId( quint32 id )
{
    this->id = id;
    emit bridgeUpdated();
}

void Bridge::setType( qpBridgeType::Type type )
{
    this->type = type;
    emit bridgeUpdated();
}

void Bridge::setNickname( QString nickname )
{
    this->nickname = nickname;
    emit bridgeUpdated();
}

void Bridge::setUsername( QString username )
{
    this->username = username;
    emit bridgeUpdated();
}

void Bridge::setResource( QString resource )
{
    this->resource = resource;
    emit bridgeUpdated();
}

void Bridge::setVendor( QString vendor )
{
    this->vendor = vendor;
    emit bridgeUpdated();
}

void Bridge::setDomain( QString domain )
{
    this->domain = domain;
    emit bridgeUpdated();
}

void Bridge::setDisabled( bool disabled )
{
    this->disabled = disabled;
    emit bridgeUpdated();
}

void Bridge::addContact( BridgeContact* contact )
{
    this->contacts.insert( contact->getID(), contact );
}

void Bridge::setOnlineStatus( qpOnlineStatus::OnlineStatus onlineStatus )
{
    this->onlineStatus = onlineStatus;
    emit bridgeUpdated();
}

quint32 Bridge::getId()
{
    return this->id;
}

qpBridgeType::Type Bridge::getType()
{
    return this->type;
}

QString Bridge::getNickname()
{
    return this->nickname;
}

QString Bridge::getUsername()
{
    return this->username;
}

QString Bridge::getResource()
{
    return this->resource;
}

QString Bridge::getVendor()
{
    return this->vendor;
}

QString Bridge::getDomain()
{
    return this->domain;
}

bool Bridge::isDisabled()
{
    return this->disabled;
}

QHash<quint64, BridgeContact*> Bridge::getContacts()
{
    return this->contacts;
}

qpOnlineStatus::OnlineStatus Bridge::getOnlineStatus()
{
    return this->onlineStatus;
}
