#include "bridgecontact.h"

BridgeContact::BridgeContact()
    : Contact()
{
    this->type = Target::BRIDGECONTACT;
}

quint32 BridgeContact::getBridgeId() const
{
    return this->bridgeId;
}

QString BridgeContact::getName() const
{
    return this->name;
}

QString BridgeContact::getCurrentMedia() const
{
    return this->currentMedia;
}

void BridgeContact::setBridgeId( quint32 bridgeId )
{
    this->bridgeId = bridgeId;
}

void BridgeContact::setName( QString name )
{
    this->name = name;
    emit nameChanged();
}

void BridgeContact::setCurrentMedia( QString currentMedia )
{
    this->currentMedia = currentMedia;
    emit currentMediaChanged();
}
