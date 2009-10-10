#include "palringoservicepane.h"

PalringoServicePane::PalringoServicePane( Service* service, QWidget* parent )
    : QPalPane( parent )
{
    this->service = service;
}

QString PalringoServicePane::getTitle() const
{
    return qpBridgeType::TypeText[ this->service->getType() ];
}
