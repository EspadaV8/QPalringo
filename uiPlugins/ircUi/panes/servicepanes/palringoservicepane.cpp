#include "palringoservicepane.h"
#include "tools.h"
#include "qptools.h"

PalringoServicePane::PalringoServicePane( Service* service, QWidget* parent )
    : ServicePane( service, parent )
{
    //
}

void PalringoServicePane::updateContacts()
{
    this->contacts = Tools::getContactListContacts();
    this->contactList->clear();

    this->contactList->setUpdatesEnabled( false );
    foreach( Contact* contact, this->contacts )
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem( this->contactList );
        twi->setText( 0, contact->getNickname() );
        twi->setIcon( 0, QPTools::getPixmap( QPTools::getTargetIcon( contact ) ) );
    }
    this->contactList->sortItems( 0, Qt::AscendingOrder );
    this->contactList->setUpdatesEnabled( true );
}
