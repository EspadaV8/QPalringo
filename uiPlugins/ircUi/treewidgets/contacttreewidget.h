#ifndef CONTACTTREEWIDGET_H
#define CONTACTTREEWIDGET_H

#include <QTreeWidget>
#include "targets/contact.h"

class ContactTreeWidget : public QTreeWidget
{
    public:
        ContactTreeWidget( QWidget* parent = 0 );
        void setContacts( QHash<quint64, Contact*> contacts );
        void updateTree();

    signals:
        void itemDoubleClicked( QTreeWidgetItem* item, int column );

    private:
        QHash<quint64, Contact*> contacts;

        QTreeWidgetItem* onlineContactList;
        QTreeWidgetItem* offlineContactList;
};

#endif // CONTACTTREEWIDGET_H
