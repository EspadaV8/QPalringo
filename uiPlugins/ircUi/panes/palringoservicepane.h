#ifndef PALRINGOSERVICEPANE_H
#define PALRINGOSERVICEPANE_H

#include "qpalpane.h"
#include "services/service.h"

class PalringoServicePane : public QPalPane
{
    public:
        PalringoServicePane( Service* service, QWidget* parent = 0 );
        virtual QString getTitle() const;

    private:
        Service* service;

};

#endif // PALRINGOSERVICEPANE_H
