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
    QHash<quint64, Contact*> contacts = Tools::getContactListContacts();
    this->contactList->setContacts( contacts );
    this->contactList->updateTree();
}
