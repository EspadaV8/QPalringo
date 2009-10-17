#include "grouppane.h"
#include "qptools.h"
#include "tools.h"

GroupPane::GroupPane( Group* group, QWidget* parent )
    : TargetPane( group, parent )
{
    this->group = group;

    this->layout = new QVBoxLayout( this );
    this->layout->setContentsMargins( 0, 0, 0, 0 );
    this->splitter = new QSplitter();
    this->groupContactsList = new QTreeWidget();
    this->groupContactsList->setHeaderHidden( true );

    this->onlineContactList = new QTreeWidgetItem( this->groupContactsList );
    this->onlineContactList->setText( 0, "Online" );
    this->onlineContactList->setExpanded( true );

    this->offlineContactList = new QTreeWidgetItem( this->groupContactsList, this->onlineContactList );
    this->offlineContactList->setText( 0, "Offline" );
    this->offlineContactList->setExpanded( false );

    this->chatView->setTitle( this->group->getDescription() );
    this->chatView->setIcon( ":/svg/group.svg" );

    this->splitter->addWidget( this->chatView );
    this->splitter->addWidget( this->groupContactsList );
    this->layout->addWidget( this->splitter );
}

QString GroupPane::getTitle() const
{
    return this->group->getTitle();
}

void GroupPane::showEvent( QShowEvent* event )
{
    TargetPane::showEvent( event );
    if( this->groupContacts.isEmpty() )
    {
        this->groupContactsList->setUpdatesEnabled( false );

        this->groupContacts = Tools::getGroupContacts( this->group->getID() );
        foreach( Contact* user, this->groupContacts )
        {
            QPixmap p = QPTools::getPixmap( QPTools::getTargetIcon( user ) );
            QTreeWidgetItem* twi = new QTreeWidgetItem();
            twi->setText( 0, user->getNickname() );
            twi->setIcon( 0, p );

            if( user->getOnlineStatus() == qpOnlineStatus::OFFLINE )
            {
                this->offlineContactList->addChild( twi );
            }
            else
            {
                this->onlineContactList->addChild( twi );
            }
        }
        this->onlineContactList->sortChildren( 0, Qt::AscendingOrder );
        this->offlineContactList->sortChildren( 0, Qt::AscendingOrder );

        this->onlineContactList->setText( 0, "Online (" + QString::number( this->onlineContactList->childCount() ) + ")" );
        this->offlineContactList->setText( 0, "Online (" + QString::number( this->offlineContactList->childCount() ) + ")" );

        this->groupContactsList->setUpdatesEnabled( true );
    }
}
