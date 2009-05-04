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

qpOnlineStatus::OnlineStatus BridgeContact::getOnlineStatus() const
{
    return this->onlineStatus;
}

QString BridgeContact::getName() const
{
    return this->name;
}

QString BridgeContact::getNickname() const
{
    return this->nickname;
}

QString BridgeContact::getStatus() const
{
    return this->status;
}

QString BridgeContact::getCurrentMedia() const
{
    return this->currentMedia;
}

void BridgeContact::setBridgeId( quint32 bridgeId )
{
    this->bridgeId = bridgeId;
}

void BridgeContact::setContactId( quint64 contactId )
{
    this->contactId = contactId;
}

void BridgeContact::setOnlineStatus( qpOnlineStatus::OnlineStatus onlineStatus )
{
    this->onlineStatus = onlineStatus;
    emit onlineStatusChanged();
}

void BridgeContact::setName( QString name )
{
    this->name = name;
    emit nameChanged();
}

void BridgeContact::setNickname( QString nickname )
{
    this->nickname = nickname;
    emit nicknameChanged();
}

void BridgeContact::setStatus( QString status )
{
    this->status = status;
    emit statusChanged();
}

void BridgeContact::setCurrentMedia( QString currentMedia )
{
    this->currentMedia = currentMedia;
    emit currentMediaChanged();
}
