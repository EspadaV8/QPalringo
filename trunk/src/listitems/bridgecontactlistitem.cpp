#include "bridgecontactlistitem.h"
#include "tools.h"

BridgeContactListItem::BridgeContactListItem( BridgeContact *contact, QWidget *parent )
    : TargetListItem( contact, parent )
{
    this->type = ListItem::BRIDGECONTACT;
    this->contact = contact;
    updateDetails();

    connect( contact, SIGNAL(updateOnlineStatus()), this, SLOT(updateOnlineStatus()));
    connect( contact, SIGNAL(nameChanged()), this, SLOT(updateDetails()));
    connect( contact, SIGNAL(updateNickname()), this, SLOT(updateDetails()));
    connect( contact, SIGNAL(updateStatusline()), this, SLOT(updateDetails()));
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
    else if( this->contact->getName().size() > 0 )
    {
        firstLine = this->contact->getName();
    }
    else
    {
        firstLine = this->contact->getUsername();
    }

    QString secondLine;
    if( this->contact->getStatusline().size() > 0 )
    {
        secondLine = this->contact->getStatusline();
    }
    else
    {
        secondLine = this->contact->getName();
    }

    this->setFirstLine( firstLine );
    this->setSecondLine( secondLine );
    this->setIcon( tools_->getTargetIcon( this->contact ) );
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
    this->updateDetails();
    emit containerGroupChanged( this );
}

void BridgeContactListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    event->accept();
    this->startChat();
}

void BridgeContactListItem::startChat()
{
    tools_->openChatWindow( this->contact );
}
