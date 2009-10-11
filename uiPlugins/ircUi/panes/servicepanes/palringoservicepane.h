#ifndef PALRINGOSERVICEPANE_H
#define PALRINGOSERVICEPANE_H

#include "servicepane.h"
#include "services/service.h"
#include "targets/contact.h"

class PalringoServicePane : public ServicePane
{
    Q_OBJECT
    public:
        PalringoServicePane( Service* service, QWidget* parent = 0 );

    public slots:
        virtual void updateContacts();

    private:
        Service* service;
        QHash<quint64, Contact*> contacts;

};

#endif // PALRINGOSERVICEPANE_H
