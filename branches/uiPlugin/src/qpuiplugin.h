#ifndef QPUIPLUGIN_H
#define QPUIPLUGIN_H

#include <QtPlugin>

class QMenu;
class QWidget;
class Group;
class Contact;

class qpUiPlugin : public QObject
{
    Q_OBJECT
    public:
        virtual ~qpUiPlugin() {}

        virtual void setUp() = 0;
        virtual void cleanUp() = 0;
        virtual QMenu* getMenu() = 0;
        virtual QWidget* getCentralWidget() = 0;
        virtual QString getName() = 0;

    public slots:
        virtual void addGroup( Group* ) = 0;
};

 Q_DECLARE_INTERFACE(qpUiPlugin, "qpalringo.qpUiPlugin/0.1")

#endif // QPUIPLUGIN_H
