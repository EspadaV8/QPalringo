#include "bridgeservice.h"

BridgeService::BridgeService()
    : Service()
{
}

QString BridgeService::getIcon() const
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
        default:
            return ":/svg/logo.svg";
    }
}
