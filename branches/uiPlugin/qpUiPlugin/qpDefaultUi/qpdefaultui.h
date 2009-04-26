#ifndef QPDEFAULTUI_H
#define QPDEFAULTUI_H

#include <QObject>
#include <QMenu>
#include <QTabWidget>

#include "qpuiplugin.h"
#include "listviews/overviewlistview.h"
#include "listviews/contactlistview.h"
#include "listviews/grouplistview.h"

class qpDefaultUi : public qpUiPlugin
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

    public slots:
        void addGroup( Group* group );

    private slots:
        void tabFocusChanged( int tabIndex );
};

#endif // QPDEFAULTUI_H
