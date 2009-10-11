#ifndef PALRINGOSERVICEPANE_H
#define PALRINGOSERVICEPANE_H

#include "servicepane.h"
#include "services/service.h"

class PalringoServicePane : public ServicePane
{
    public:
        PalringoServicePane( Service* service, QWidget* parent = 0 );

    private:
        Service* service;

};

#endif // PALRINGOSERVICEPANE_H
