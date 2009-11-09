#include "servicepane.h"

ServicePane::ServicePane( Service* service, QWidget* parent )
    : QPalPane( parent )
{
    this->service = service;
    this->layout = new QVBoxLayout( this );
    this->splitter = new QSplitter;

    this->contactList = new ContactTreeWidget();

    this->splitter->addWidget( this->contactList );

    this->layout->addWidget( this->splitter );
}

QString ServicePane::getTitle() const
{
    return qpBridgeType::TypeText[ this->service->getType() ];
}
