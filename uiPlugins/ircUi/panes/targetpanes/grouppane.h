#ifndef GROUPPANE_H
#define GROUPPANE_H

#include <QtGui>
#include "targetpane.h"
#include "chatview.h"
#include "targets/group.h"
#include "targets/contact.h"

class GroupPane : public TargetPane
{
    Q_OBJECT
    public:
        GroupPane( Group* group = 0, QWidget* parent = 0 );
        virtual QString getTitle() const;

    protected:
        void showEvent( QShowEvent* event );
        void hideEvent( QHideEvent* event );

    private:
        Group* group;
        QHash<quint64, Contact*> groupContacts;
        QVBoxLayout* layout;
        QSplitter* splitter;
        QListWidget* groupContactsList;

        void insertMessages();

    private slots:
        void handleNewMessage();
        void sendTextMessage( QString text );
};

#endif // GROUPPANE_H
