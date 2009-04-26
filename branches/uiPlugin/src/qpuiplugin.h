#ifndef QPUIPLUGIN_H
#define QPUIPLUGIN_H

#include <QtPlugin>

class QMenu;
class QWidget;

class qpUiPlugin
{
    public:
        virtual ~qpUiPlugin() {}

        virtual void setUp() = 0;
        virtual void cleanUp() = 0;
        virtual QMenu* getMenu() = 0;
        virtual QWidget* getCentralWidget() = 0;
        virtual QString getName() = 0;
};

 Q_DECLARE_INTERFACE(qpUiPlugin, "qpalringo.qpUiPlugin/0.1")

#endif // QPUIPLUGIN_H
