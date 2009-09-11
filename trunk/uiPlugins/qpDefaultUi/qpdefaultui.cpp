#include <QtGui>
#include "qpdefaultui.h"
#include "listviews/overviewlistview.h"
//#include "tools.h"

QWidget* QPDefaultUi::getCentralWidget()
{
    this->mainTabs = new QTabWidget();
    /*
    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));
*/
    OverviewListView *overviewList = new OverviewListView( mainTabs );
    overviewList->setupContainers();

    //mainTabs->addTab( overviewList, tools_->getPixmap( ":/svg/palringoService.svg" ), tr( "Overview" ) );
    mainTabs->addTab( overviewList, tr( "Overview" ) );
/*
    ContactListView *contactList = new ContactListView( mainTabs );
    contactList->setupContainers();

    mainTabs->addTab( contactList, tools_->getPixmap( ":/svg/onlineContact.svg" ), tr( "&Contacts" ) );

    setCentralWidget( mainTabs );

    connect( tools_, SIGNAL( newGroupAdded( Group* )), this, SLOT( newGroupAdded( Group* ) ) );
    connect( tools_, SIGNAL( groupLeft( quint64 ) ), this, SLOT( groupLeft( quint64 ) ) );
    connect( tools_, SIGNAL( cleanUp() ), this, SLOT( cleanUp() ) );
    */
    return this->mainTabs;
}

QString QPDefaultUi::getName()
{
    return "Default plugin";
}

Q_EXPORT_PLUGIN2(qpdefaultui, QPDefaultUi)
