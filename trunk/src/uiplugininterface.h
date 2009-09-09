#ifndef UIPLUGININTERFACE_H
#define UIPLUGININTERFACE_H

#include <QtGui>

class UiPluginInterface
{
    public:
        virtual ~UiPluginInterface() {}
        virtual QWidget* getCentralWidget() = 0;
        virtual QString getName() = 0;
};

Q_DECLARE_INTERFACE(UiPluginInterface, "qpalringo.UiPluginInterface/0.1")

#endif // UIPLUGININTERFACE_H
