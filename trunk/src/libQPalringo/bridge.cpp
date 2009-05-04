#include "bridge.h"

Bridge::Bridge()
{
}


void Bridge::setId( quint32 id )
{
    this->id = id;
}

void Bridge::setType( qpBridgeType::Type type )
{
    this->type = type;
}

void Bridge::setNickname( QString nickname )
{
    this->nickname = nickname;
}

void Bridge::setUsername( QString username )
{
    this->username = username;
}

void Bridge::setResource( QString resource )
{
    this->resource = resource;
}

void Bridge::setVendor( QString vendor )
{
    this->vendor = vendor;
}

void Bridge::setDomain( QString domain )
{
    this->domain = domain;
}

void Bridge::setDisabled( bool disabled )
{
    this->disabled = disabled;
}

void Bridge::addContact( BridgeContact* contact )
{
    this->contacts.insert( contact->getID(), contact );
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
