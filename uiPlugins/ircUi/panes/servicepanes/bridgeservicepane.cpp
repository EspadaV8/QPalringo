#include "bridgeservicepane.h"
#include "tools.h"
#include "qptools.h"

BridgeServicePane::BridgeServicePane( Bridge* bridge, QWidget* parent )
    : ServicePane( bridge, parent )
{
    this->bridge = bridge;
}

void BridgeServicePane::updateContacts()
{
    this->contactList->setUpdatesEnabled( false );

    foreach( Contact* contact, this->bridge->getContacts() )
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem();
        twi->setText( 0, contact->getNickname() );
        twi->setData( 1, Qt::DisplayRole, contact->getID() );
        twi->setIcon( 0, QPTools::getPixmap( QPTools::getTargetIcon( contact ) ) );
        if( contact->getOnlineStatus() == qpOnlineStatus::OFFLINE )
        {
            this->offlineContactList->addChild( twi );
        }
        else
        {
            this->onlineContactList->addChild( twi );
        }
    }

    this->onlineContactList->sortChildren( 0, Qt::AscendingOrder );
    this->offlineContactList->sortChildren( 0, Qt::AscendingOrder );
    this->contactList->setUpdatesEnabled( true );
}

