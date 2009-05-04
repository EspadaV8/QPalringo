#include "bridgecontactlistitem.h"
#include "tools.h"

BridgeContactListItem::BridgeContactListItem( QWidget *parent, BridgeContact *contact )
    : ListItem( parent )
{
    this->type = ListItem::BRIDGECONTACT;
    this->contact = contact;
    updateDetails();

    connect( contact, SIGNAL(onlineStatusChanged()), this, SLOT(updateOnlineStatus()));
    connect( contact, SIGNAL(nameChanged()), this, SLOT(updateDetails()));
    connect( contact, SIGNAL(nicknameChanged()), this, SLOT(updateDetails()));
    connect( contact, SIGNAL(statusChanged()), this, SLOT(updateDetails()));
    connect( contact, SIGNAL(currentMediaChanged()), this, SLOT(updateDetails()));

    emit( containerGroupChanged( this ) );
}

void BridgeContactListItem::updateDetails()
{
    QString firstLine;
    if( this->contact->getNickname().size() > 0 )
    {
        firstLine = this->contact->getNickname();
    }
    else
    {
        firstLine = this->contact->getName();
    }

    QString secondLine;
    if( this->contact->getStatus().size() > 0 )
    {
        secondLine = this->contact->getStatus();
    }
    else
    {
        secondLine = this->contact->getName();
    }

    this->setFirstLine( firstLine );
    this->setSecondLine( secondLine );
    this->setIcon( this->getIcon() );
}

QString BridgeContactListItem::getIcon()
{
    QString icon = ":/services/";

    Bridge* b = tools_->getBridge( this->contact->getBridgeId() );
    if( b != NULL )
    {
        switch( b->getType() )
        {
            case qpBridgeType::AIM:
                icon += "aim";
                break;
            case qpBridgeType::FACEBOOK:
                icon += "facebook";
                break;
            case qpBridgeType::GADUGADU:
                icon += "gaduGadu";
                break;
            case qpBridgeType::ICQ:
                icon += "icq";
                break;
            case qpBridgeType::MSN:
                icon += "msn";
                break;
            case qpBridgeType::QQ:
                icon += "qq";
                break;
            case qpBridgeType::XMPP:
                icon += "jabber";
                break;
            case qpBridgeType::YAHOO:
                icon += "yahoo";
                break;
            default:
                icon = ":/svg/logo";
                break;
        }
    }
    else
    {
        icon = ":/svg/logo";
    }

    if( this->contact->getOnlineStatus() == qpOnlineStatus::OFFLINE )
    {
        icon += "_offline";
    }

    icon += ".svg";

    return icon;
}

QString BridgeContactListItem::getContainerGroup()
{
    if( this->contact->getOnlineStatus() == qpOnlineStatus::OFFLINE )
    {
        return "Offline";
    }
    else
    {
        return "Online";
    }
}

void BridgeContactListItem::updateOnlineStatus()
{
    this->setIcon( this->getIcon() );
    emit containerGroupChanged( this );
}
