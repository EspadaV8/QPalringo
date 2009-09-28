#ifndef QPDEFAULTUI_H
#define QPDEFAULTUI_H

#include <QtGui>
#include "uiplugininterface.h"
#include "targets/target.h"
#include "targets/group.h"
#include "chatwindow/chatwindow.h"
#include "listviews/overviewlistview.h"

class QPDefaultUi : public QObject, UiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(UiPluginInterface)

    public:
        QWidget* getCentralWidget();
        QString getName();

    private slots:
        void newGroupAdded( Group *group );
        void tabFocusChanged( int tabIndex );

        void focusChatWindow( Target* target );
        void removeChatWindow( Target *target );
        void newPendingMessage( Target* target );

    private:
        QTabWidget* mainTabs;
        OverviewListView* overviewList;
        QHash<Target*, ChatWindow *> openWindows;

        bool checkChatWindowOpen( Target *target );
};

#endif // QPDEFAULTUI_H
