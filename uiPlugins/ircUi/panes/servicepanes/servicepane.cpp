#include "servicepane.h"

ServicePane::ServicePane( Service* service, QWidget* parent )
    : QPalPane( parent )
{
    this->service = service;
    this->layout = new QVBoxLayout( this );
    this->splitter = new QSplitter;

    this->contactList = new QTreeWidget;
    this->onlineContactList = new QTreeWidgetItem( this->contactList );
    this->onlineContactList->setText( 0, "Online" );
    this->offlineContactList = new QTreeWidgetItem( this->contactList, this->onlineContactList );
    this->offlineContactList->setText( 0, "Offline" );

    this->splitter->addWidget( this->contactList );

    this->layout->addWidget( this->splitter );
}

QString ServicePane::getTitle() const
{
    return qpBridgeType::TypeText[ this->service->getType() ];
}
