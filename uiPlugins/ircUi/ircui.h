#ifndef IRCUI_H
#define IRCUI_H

#include <QtGui>
#include "uiplugininterface.h"
#include "qpdefaultui.h"
#include "accountlist.h"
#include "panes/qpalpane.h"
#include "panes/overviewpane.h"
#include "services/service.h"
#include "targets/group.h"

class IrcUi : public QPDefaultUi
{
    Q_OBJECT
    Q_INTERFACES(UiPluginInterface)

    public:
        IrcUi();
        virtual void setup();
        virtual QWidget* getCentralWidget();
        virtual QString getName();

        static void addText( QString text );

    private slots:
        void changeVisiblePane( int paneId );
        void changeVisiblePane( QTreeWidgetItem* current, QTreeWidgetItem* previous );
        void saveSplitterSize( int index, int pos );
        /*
        void newGroupAdded( Group *group );
        void tabFocusChanged( int tabIndex );

        void focusChatWindow( Target* target );
        void removeChatWindow( Target *target );
        void newPendingMessage( Target* target );
        void sendMessage( ChatWindow* chatwindow, Target* target, Message message );
*/
    private:
        QSplitter* splitter;
        QStackedWidget* panes;
        QTreeWidgetItem* servicesTreeItem;
        QTreeWidgetItem* openChatsList;
        QTreeWidgetItem* groupsList;

        AccountList* accountList;
        static OverviewPane* overviewPane;

        QHash<QTreeWidgetItem*, QPalPane*> treeWidgetToPane;
        QHash<QTreeWidgetItem*, Service*> treeWidgetToService;
        QHash<QTreeWidgetItem*, Target*> treeWidgetToTarget;

        void insertPane( QTreeWidgetItem* twi, QPalPane* pane );

    private slots:
        void gotService( Service* service );
        void gotGroup( Group* group );
        void itemDoubleClicked( QTreeWidgetItem* item, int column );
        void updateTargetIcon( Target* target );
        void openChat( Target* );
        void gotContact( Contact* contact );
        /*
        QTabWidget* mainTabs;
        OverviewListView* overviewList;
        QHash<Target*, ChatWindow *> openWindows;

        bool checkChatWindowOpen( Target *target );
        */
};

#endif // IRCUI_H
