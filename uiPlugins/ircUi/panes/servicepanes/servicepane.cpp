#include "servicepane.h"

ServicePane::ServicePane( Service* service, QWidget* parent )
    : QPalPane( parent )
{
    this->service = service;
}

QString ServicePane::getTitle() const
{
    return qpBridgeType::TypeText[ this->service->getType() ];
}
