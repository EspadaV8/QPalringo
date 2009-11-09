#include "contacttreewidget.h"
#include "qptools.h"

ContactTreeWidget::ContactTreeWidget( QWidget* parent )
    : QTreeWidget( parent )
{
    this->setHeaderHidden( true );
    this->onlineContactList = new QTreeWidgetItem( this );
    this->onlineContactList->setText( 0, "Online" );
    this->onlineContactList->setExpanded( true );
    this->offlineContactList = new QTreeWidgetItem( this, this->onlineContactList );
    this->offlineContactList->setText( 0, "Offline" );
    this->offlineContactList->setExpanded( false );
}

void ContactTreeWidget::itemDoubleClicked( QTreeWidgetItem* item, int /* column */ )
{
    Contact* c = this->contacts.value( item->data( 1, Qt::DisplayRole ).toInt() );

    if( c )
    {
        emit c->chatRequested();
    }
}

void ContactTreeWidget::setContacts( QHash<quint64, Contact*> contacts )
{
    this->contacts = contacts;
}

void ContactTreeWidget::updateTree()
{
    bool onlineExpanded = this->onlineContactList->isExpanded();
    bool offlineExpanded = this->offlineContactList->isExpanded();

    this->setUpdatesEnabled( false );

    this->onlineContactList->takeChildren();
    this->offlineContactList->takeChildren();

    foreach( Contact* contact, this->contacts )
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem();
        twi->setText( 0, contact->getNickname() );
        twi->setData( 1, Qt::DisplayRole, contact->getID() );
        twi->setIcon( 0, QPTools::getPixmap( QPTools::getTargetIcon( contact ) ) );
        if( contact->getOnlineStatus() == qpOnlineStatus::OFFLINE )
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

    this->onlineContactList->setExpanded( onlineExpanded );
    this->offlineContactList->setExpanded( offlineExpanded );
    this->setUpdatesEnabled( true );
}
