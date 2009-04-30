#include "qpenhancedui.h"
#include "tools.h"

void qpEnhancedUi::setUp()
{
    uitools_ = new uiTools();

    mainTabs = new QTabWidget();
    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));

    overviewList = new OverviewListView( mainTabs );
    overviewList->setupContainers();
    connect(overviewList, SIGNAL( startChat( Target* ) ), uitools_, SLOT( openChatWindow( Target* ) ) );

    contactList = new ContactListView( mainTabs );
    contactList->setupContainers();
    connect(contactList, SIGNAL( startChat( Target* ) ), uitools_, SLOT( openChatWindow( Target* ) ) );

    mainTabs->addTab( overviewList, tools_->getPixmap( ":/svg/logo.svg" ), tr( "Overview" ) );
    mainTabs->addTab( contactList, tools_->getPixmap( ":/svg/onlineContact.svg" ), tr( "&Contacts" ) );

    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));
    connect( tools_, SIGNAL( newPendingMessage( Target* ) ), this, SLOT( newPendingMessage( Target* ) ) );
}

void qpEnhancedUi::tabFocusChanged( int tabIndex )
{
    PalringoListView *p = qobject_cast<PalringoListView*>( this->mainTabs->widget(tabIndex) );
    if( p )
    {
        emit p->inFocus();
    }
}

void qpEnhancedUi::removeGroup( quint64 groupId )
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

void qpEnhancedUi::cleanUp()
{
    for( int i = mainTabs->count(); i > 1; i-- )
    {
        QWidget *w = mainTabs->widget( i );
        mainTabs->removeTab( i );
        delete w;
    }
}

void qpEnhancedUi::addGroup( Group* group )
{
    GroupListView *groupTab = new GroupListView( mainTabs, group );
    groupTab->setupContainers();
    connect(groupTab, SIGNAL( startChat( Target* ) ), uitools_, SLOT( openChatWindow( Target* ) ) );
    mainTabs->addTab( groupTab, tools_->getPixmap( ":/svg/group.svg" ), group->getName() );
}

QMenu* qpEnhancedUi::getMenu()
{
}

QWidget* qpEnhancedUi::getCentralWidget()
{
    return mainTabs;
}

QString qpEnhancedUi::getName()
{
    return "QPalringo Enhanced UI";
}


void qpEnhancedUi::newPendingMessage( Target* target )
{
    qDebug( "newPendingMessage" );
    QSettings settings;

    if( ( ( target->getType() == Target::CONTACT && settings.value( "alerts/privateAutoOpen" ).toBool() ) ||
          ( target->getType() == Target::GROUP   && settings.value( "alerts/groupAutoOpen" ).toBool() ) ) &&
        !uitools_->checkChatWindowOpen( target ) )
    {
        uitools_->openChatWindow( target );
    }
    else
    {
        this->overviewList->newPendingMessage( target );
    }
}

Q_EXPORT_PLUGIN2(qp_enhanced_ui, qpEnhancedUi)