#ifndef QPDEFAULTUI_H
#define QPDEFAULTUI_H

#include <QtGui>
#include "uiplugininterface.h"
#include "targets/group.h"

class QPDefaultUi : public QObject, UiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(UiPluginInterface)

    public:
        QWidget* getCentralWidget();
        QString getName();

    private slots:
        void newGroupAdded( Group *group );
        void tabFocusChanged(int tabIndex );

    private:
        QTabWidget* mainTabs;
};

#endif // QPDEFAULTUI_H
