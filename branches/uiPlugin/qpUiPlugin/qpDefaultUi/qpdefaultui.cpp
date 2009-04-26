#include "qpdefaultui.h"

void qpDefaultUi::setUp()
{
    mainTabs = new QTabWidget();
    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));

    OverviewListView *overviewList = new OverviewListView( mainTabs );
    overviewList->setupContainers();

    ContactListView *contactList = new ContactListView( mainTabs );
    contactList->setupContainers();

    mainTabs->addTab( overviewList, tr( "Overview" ) );
    mainTabs->addTab( contactList, tr( "&Contacts" ) );

    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));
}

void qpDefaultUi::tabFocusChanged( int tabIndex )
{
    PalringoListView *p = qobject_cast<PalringoListView*>( this->mainTabs->widget(tabIndex) );
    if( p )
    {
        emit p->inFocus();
    }
}

void qpDefaultUi::cleanUp()
{
    for( int i = mainTabs->count(); i > 1; i-- )
    {
        QWidget *w = mainTabs->widget( i );
        mainTabs->removeTab( i );
        delete w;
    }
}

QMenu* qpDefaultUi::getMenu()
{
}

QWidget* qpDefaultUi::getCentralWidget()
{
    return mainTabs;
}

QString qpDefaultUi::getName()
{
    return "QPalringo Default UI";
}


Q_EXPORT_PLUGIN2(qp_default_ui, qpDefaultUi)
