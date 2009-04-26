#ifndef QPENHANCEDUI_H
#define QPENHANCEDUI_H

#include <QObject>
#include <QMenu>
#include <QTabWidget>

#include "uitools.h"
#include "qpuiplugin.h"
#include "defaultListViews/overviewlistview.h"
#include "defaultListViews/contactlistview.h"
#include "defaultListViews/grouplistview.h"

class qpEnhancedUi : public qpUiPlugin
{
    Q_OBJECT
    Q_INTERFACES(qpUiPlugin)

    public:
        void setUp();
        void cleanUp();
        QMenu* getMenu();
        QWidget* getCentralWidget();
        QString getName();

    private:
        QTabWidget *mainTabs;
        QWidget *overviewPage;
        QWidget *contactsPage;

        OverviewListView* overviewList;
        ContactListView* contactList;

    public slots:
        void addGroup( Group* group );
        void removeGroup( quint64 groupId );
        void newPendingMessage( Target* target );

    private slots:
        void tabFocusChanged( int tabIndex );
};

#endif // QPENHANCEDUI_H
