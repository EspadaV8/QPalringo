#include "bridgeservice.h"
#include "tools.h"

BridgeService::BridgeService()
    : Service()
{
}

BridgeService::BridgeService( Bridge* bridge )
    : Service()
{
    this->bridge = bridge;
    connect( bridge, SIGNAL( onlineStatusChanged() ), this, SIGNAL( serviceUpdated() ) );
}

void BridgeService::loginService()
{
    tools_->setBridgeStatus( this->bridge->getId(), qpOnlineStatus::ONLINE );
}

qpOnlineStatus::OnlineStatus BridgeService::getOnlineStatus() const
{
    return this->bridge->getOnlineStatus();
}

QString BridgeService::getNickname() const
{
    return this->bridge->getNickname();
}

QString BridgeService::getStatus() const
{
    return qpOnlineStatus::OnlineStatusText[ this->bridge->getOnlineStatus() ];
}

QString BridgeService::getIcon() const
{
    if( this->bridge->getOnlineStatus() == qpOnlineStatus::OFFLINE )
    {
        switch( this->type )
        {
            case qpBridgeType::MSN:
                return ":/services/msn_offline.svg";
            case qpBridgeType::YAHOO:
                return ":/services/yahoo_offline.svg";
            case qpBridgeType::AIM:
                return ":/services/aim_offline.svg";
            case qpBridgeType::GADUGADU:
                return ":/services/gaduGadu_offline.svg";
            case qpBridgeType::ICQ:
                return ":/services/icq_offline.svg";
            case qpBridgeType::XMPP:
                return ":/services/jabber_offline.svg";
            case qpBridgeType::QQ:
                return ":/services/qq_offline.svg";
            case qpBridgeType::FACEBOOK:
                return ":/services/facebook_offline.svg";
            default:
                return ":/svg/logo_offline.svg";
        }
    }
    else
    {
        switch( this->type )
        {
            case qpBridgeType::MSN:
                return ":/services/msn.svg";
            case qpBridgeType::YAHOO:
                return ":/services/yahoo.svg";
            case qpBridgeType::AIM:
                return ":/services/aim.svg";
            case qpBridgeType::GADUGADU:
                return ":/services/gaduGadu.svg";
            case qpBridgeType::ICQ:
                return ":/services/icq.svg";
            case qpBridgeType::XMPP:
                return ":/services/jabber.svg";
            case qpBridgeType::QQ:
                return ":/services/qq.svg";
            case qpBridgeType::FACEBOOK:
                return ":/services/facebook.svg";
            default:
                return ":/svg/logo.svg";
        }
    }
}

quint32 BridgeService::getBridgeId()
{
    return this->bridge->getId();
}
