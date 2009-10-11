#ifndef SERVICEPANE_H
#define SERVICEPANE_H

#include <QtGui>
#include "../qpalpane.h"
#include "services/service.h"

class ServicePane : public QPalPane
{
    Q_OBJECT
    public:
        ServicePane( Service* service = 0, QWidget* parent = 0 );
        virtual QString getTitle() const;

    public slots:
        virtual void updateContacts() = 0;

    protected:
        Service* service;
        QVBoxLayout* layout;
        QSplitter* splitter;
        QTreeWidget* contactList;
};

#endif // SERVICEPANE_H
