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

void qpDefaultUi::removeGroup( quint64 groupId )
{
    for( int i = 0; i < mainTabs->count(); i++ )
    {
        GroupListView *w = qobject_cast<GroupListView*>( mainTabs->widget( i ) );
        if( w && ( w->getGroup()->getID() == groupId ) )
        {
            if( i == mainTabs->currentIndex() )
            {
                mainTabs->setCurrentIndex( 0 );
            }
            mainTabs->removeTab( i );
            w->deleteLater();
            break;
        }
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

void qpDefaultUi::addGroup( Group* group )
{
    GroupListView *groupTab = new GroupListView( mainTabs, group );
    groupTab->setupContainers();
    mainTabs->addTab( groupTab, group->getName() );
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
