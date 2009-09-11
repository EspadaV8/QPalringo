#ifndef QPDEFAULTUI_H
#define QPDEFAULTUI_H

#include <QtGui>
#include "uiplugininterface.h"

class QPDefaultUi : public QObject, UiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(UiPluginInterface)

    public:
        QWidget* getCentralWidget();
        QString getName();

    private:
        QTabWidget* mainTabs;
};

#endif // QPDEFAULTUI_H
