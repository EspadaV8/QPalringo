#include <QtGui>
#include "qpdefaultui.h"
#include "listviews/overviewlistview.h"
#include "listviews/contactlistview.h"
#include "tools.h"

QWidget* QPDefaultUi::getCentralWidget()
{
    this->mainTabs = new QTabWidget();

    OverviewListView* overviewList = new OverviewListView( mainTabs, tools() );
    overviewList->setupContainers();

    mainTabs->addTab( overviewList, Tools::getPixmap( ":/svg/palringoService.svg" ), tr( "Overview" ) );

    ContactListView* contactList = new ContactListView( mainTabs, tools() );
    contactList->setupContainers();

    mainTabs->addTab( contactList, Tools::getPixmap( ":/svg/onlineContact.svg" ), tr( "&Contacts" ) );
    /*
    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));
    connect( tools_, SIGNAL( newGroupAdded( Group* )), this, SLOT( newGroupAdded( Group* ) ) );
    connect( tools_, SIGNAL( groupLeft( quint64 ) ), this, SLOT( groupLeft( quint64 ) ) );
    connect( tools_, SIGNAL( cleanUp() ), this, SLOT( cleanUp() ) );
    */

    connect( overviewList, SIGNAL(signinPalringo(QString, QString )), tools(), SLOT( openPalringoConnection( QString, QString ) ));

    //connect( tools_, SIGNAL( gotBridgeContact( BridgeContact* ) ), this, SLOT( bridgeContactReceived( BridgeContact* ) ) );

    return this->mainTabs;
}

QString QPDefaultUi::getName()
{
    return "Default plugin";
}

Q_EXPORT_PLUGIN2(qpdefaultui, QPDefaultUi)
