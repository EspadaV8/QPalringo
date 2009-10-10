#include "ircui.h"
#include "tools.h"
#include "qptools.h"
#include "qpdefaultui.h"
#include "listviews/contactlistview.h"
#include "listviews/grouplistview.h"
#include "panes/targetpanes/grouppane.h"
#include <QDebug>

OverviewPane* IrcUi::overviewPane = NULL;

IrcUi::IrcUi()
{
    this->splitter = new QSplitter();
    this->panes = new QStackedWidget();

    this->accountList = new AccountList( this->splitter );

    this->servicesTreeItem = new QTreeWidgetItem( this->accountList );
    this->servicesTreeItem->setText(0, tr("Services"));
    this->servicesTreeItem->setExpanded( true );

    groups = new QTreeWidgetItem( this->accountList, this->servicesTreeItem );
    groups->setText(0, tr("Groups"));
    groups->setExpanded( true );

    this->overviewPane = new OverviewPane();
}

QString IrcUi::getName()
{
    return "IRC style plugin";
}

void IrcUi::setup()
{
    this->insertPane( this->servicesTreeItem, this->overviewPane );

    this->splitter->setStretchFactor( 1, 1 );
    this->splitter->addWidget( this->accountList );
    this->splitter->addWidget( this->panes );

    connect( this->accountList, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* )),
             this, SLOT( changeVisiblePane( QTreeWidgetItem*, QTreeWidgetItem* ) ) );

    connect( this->accountList, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ),
             this, SLOT( itemDoubleClicked( QTreeWidgetItem*, int ) ) );

    connect( this->tools(), SIGNAL( gotServiceDetails( Service* ) ),
             this,          SLOT( gotService( Service* ) ) );

    connect( this->tools(), SIGNAL( newGroupAdded( Group* ) ),
             this,          SLOT( gotGroup( Group* ) ) );
}

void IrcUi::gotService( Service* service )
{
    QTreeWidgetItem* twi = new QTreeWidgetItem( this->servicesTreeItem );
    twi->setText(0, service->getNickname() );
    QPixmap p( QPTools::getServiceIcon( service ) );
    twi->setIcon( 0, p );

    this->treeWidgetToService.insert( twi, service );
}

void IrcUi::gotGroup( Group* group )
{
    QTreeWidgetItem* twi = new QTreeWidgetItem( this->groups );
    twi->setText(0, group->getName() );
    QPixmap p( QPTools::getPixmap( ":/svg/group.svg" ) );
    twi->setIcon( 0, p );

    this->groups->sortChildren( 0, Qt::AscendingOrder );
    this->treeWidgetToTarget.insert( twi, group );

    GroupPane* gp = new GroupPane( group );
    this->insertPane( twi, gp );

    connect( gp, SIGNAL(updateTargetIcon( Target* )), this, SLOT(updateTargetIcon( Target* )));
}

void IrcUi::updateTargetIcon( Target* target )
{
    QTreeWidgetItem* item = this->treeWidgetToTarget.key( target );
    if( item )
    {
        QPixmap p;
        if( target->getPendingMessages().count() > 0 )
        {
            p = QPTools::getPixmap( ":/svg/text.svg" );
        }
        else
        {
            p = QPTools::getPixmap( QPTools::getTargetIcon( target ) );
        }
        item->setIcon( 0, p );
    }
}

void IrcUi::itemDoubleClicked( QTreeWidgetItem* item, int /* column */ )
{
    if( this->treeWidgetToService.contains( item ) )
    {
        Service* service = this->treeWidgetToService.value( item );
        qDebug() << service->getType();
        if( service->getType() == qpBridgeType::PALRINGO )
        {
            SigninWindow* sw = new SigninWindow();
            sw->show();

            connect( sw, SIGNAL( signin( QString, QString ) ),
                     tools(), SLOT( openPalringoConnection( QString, QString ) ) );
        }
        else
        {
            //
        }
    }
}

void IrcUi::insertPane( QTreeWidgetItem* twi, QPalPane* pane )
{
    this->panes->addWidget( pane );
    this->treeWidgetToPane.insert( twi, pane );
}

void IrcUi::changeVisiblePane( QTreeWidgetItem* current, QTreeWidgetItem* /* previous */ )
{
    if( this->treeWidgetToPane.contains( current ) )
    {
        this->addText( "Changing pane" );
        this->panes->setCurrentWidget( this->treeWidgetToPane.value( current ) );
    }
    else
    {
        this->addText( "Tried to access pane that doesn't exist" );
        /*
        if( this->treeWidgetToPane.contains( previous ) )
        {
            this->accountList->setCurrentItem( previous );
        }
        */
    }
}

QWidget* IrcUi::getCentralWidget()
{
    return this->splitter;
}

void IrcUi::addText( QString text )
{
    IrcUi::overviewPane->addText( text );
}

Q_EXPORT_PLUGIN2(ircui, IrcUi)