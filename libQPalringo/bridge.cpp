#include "bridge.h"

Bridge::Bridge()
{
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
