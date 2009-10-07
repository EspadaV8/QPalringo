#ifndef UIPLUGININTERFACE_H
#define UIPLUGININTERFACE_H

#include <QtGui>

//class PalringoWindow;
class Tools;

class UiPluginInterface
{
    public:
        virtual ~UiPluginInterface() {}
        virtual void setup() = 0;
        virtual QWidget* getCentralWidget() = 0;
        virtual QString getName() = 0;

//        void setPalringoWindow(PalringoWindow* w) {palringoWindow_ = w;}
        void setTools(Tools* t) {tools_ = t;}

//        PalringoWindow* palringoWindow() { return this->palringoWindow_; }
        Tools* tools() { return this->tools_; }

    private:
//        PalringoWindow *palringoWindow_;
        Tools *tools_;
};

Q_DECLARE_INTERFACE(UiPluginInterface, "qpalringo.UiPluginInterface/0.1")

#endif // UIPLUGININTERFACE_H
