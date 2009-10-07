#ifndef QPDEFAULTUI_H
#define QPDEFAULTUI_H

#include <QtGui>
#include "uiplugininterface.h"
#include "targets/target.h"
#include "targets/group.h"
#include "chatwindow/chatwindow.h"
#include "listviews/overviewlistview.h"

class QPDefaultUi : public QObject, protected UiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(UiPluginInterface)

    public:
        virtual void setup();
        virtual QWidget* getCentralWidget();
        virtual QString getName();

    protected slots:
        virtual void newGroupAdded( Group *group );
        virtual void tabFocusChanged( int tabIndex );

        virtual void focusChatWindow( Target* target );
        virtual void removeChatWindow( Target *target );
        virtual void newPendingMessage( Target* target );
        virtual void sendMessage( ChatWindow* chatwindow, Target* target, Message message );

    protected:
        QTabWidget* mainTabs;
        OverviewListView* overviewList;
        QHash<Target*, ChatWindow *> openWindows;

        virtual bool checkChatWindowOpen( Target *target );
};

#endif // QPDEFAULTUI_H
