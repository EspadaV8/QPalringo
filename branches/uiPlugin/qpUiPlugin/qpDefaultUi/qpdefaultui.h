#ifndef QPDEFAULTUI_H
#define QPDEFAULTUI_H

#include <QObject>
#include <QMenu>
#include <QTabWidget>

#include "qpuiplugin.h"
#include "listviews/overviewlistview.h"
#include "listviews/contactlistview.h"

class qpDefaultUi : public QObject, public qpUiPlugin
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


    private slots:
        void tabFocusChanged( int tabIndex );
};

#endif // QPDEFAULTUI_H
