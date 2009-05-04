#include "bridgecontact.h"

BridgeContact::BridgeContact()
    : Target()
{
    this->type = Target::BRIDGECONTACT;
}

quint32 BridgeContact::getBridgeId() const
{
    return this->bridgeId;
}

quint64 BridgeContact::getContactId() const
{
    return this->contactId;
}

QString BridgeContact::getName() const
{
    return this->name;
}

QString BridgeContact::getNickname() const
{
    return this->nickname;
}

void BridgeContact::setBridgeId( quint32 bridgeId )
{
    this->bridgeId = bridgeId;
}

void BridgeContact::setContactId( quint64 contactId )
{
    this->contactId = contactId;
}

void BridgeContact::setName( QString name )
{
    this->name = name;
}

void BridgeContact::setNickname( QString nickname )
{
    this->nickname = nickname;
}
