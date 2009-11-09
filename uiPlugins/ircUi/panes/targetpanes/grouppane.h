#ifndef GROUPPANE_H
#define GROUPPANE_H

#include <QtGui>
#include "targetpane.h"
#include "chatview.h"
#include "targets/group.h"
#include "targets/contact.h"
#include "treewidgets/contacttreewidget.h"

class GroupPane : public TargetPane
{
    Q_OBJECT
    public:
        GroupPane( Group* group = 0, QWidget* parent = 0 );
        virtual QString getTitle() const;

    protected:
        void showEvent( QShowEvent* event );

    private:
        Group* group;
        QHash<quint64, Contact*> groupContacts;
        QVBoxLayout* layout;
        QSplitter* splitter;
        ContactTreeWidget* groupContactsList;
        QTreeWidgetItem* onlineContactList;
        QTreeWidgetItem* offlineContactList;
};

#endif // GROUPPANE_H
