#ifndef SERVICEPANE_H
#define SERVICEPANE_H

#include <QtGui>
#include "../qpalpane.h"
#include "services/service.h"

class ServicePane : public QPalPane
{
    public:
        ServicePane( Service* service = 0, QWidget* parent = 0 );
        virtual QString getTitle() const;

    protected:
        Service* service;
};

#endif // SERVICEPANE_H
