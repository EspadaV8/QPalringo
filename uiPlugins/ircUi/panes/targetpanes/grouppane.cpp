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

    this->onlineContacts = new QTreeWidgetItem( this->groupContactsList );
    this->onlineContacts->setText( 0, "Online" );
    this->onlineContacts->setExpanded( true );

    this->offlineContacts = new QTreeWidgetItem( this->groupContactsList, this->onlineContacts );
    this->offlineContacts->setText( 0, "Offline" );
    this->offlineContacts->setExpanded( false );


    this->chatView->setTitle( this->group->getDescription() );
    this->chatView->setIcon( ":/svg/group.svg" );

    this->splitter->addWidget( this->chatView );
    this->splitter->addWidget( this->groupContactsList );
    this->layout->addWidget( this->splitter );

    connect( this->group, SIGNAL( pendingMessage() ), this, SLOT( handleNewMessage() ) );
    connect( this->chatView, SIGNAL(sendMessage(QString)), this, SLOT( sendTextMessage( QString ) ) );
}

QString GroupPane::getTitle() const
{
    return this->group->getTitle();
}

void GroupPane::handleNewMessage()
{
    if( this->isHidden() )
    {
        emit updateTargetIcon( this->group );
    }
    else
    {
        this->insertMessages();
    }
}

void GroupPane::insertMessages()
{
    foreach( Message message, this->group->getPendingMessages() )
    {
        this->chatView->appendMessage( message );
    }
    this->group->clearPending();

    emit updateTargetIcon( this->group );
}

void GroupPane::sendTextMessage( QString text )
{
    Message message = QPTools::createTextMessage( text );
    if( message.senderID() == 0 )
    {
        quint64 userId = Tools::getUser().userID;
        message.setSenderID( userId );
        message.setSender( Tools::getContact( userId ) );
    }
    Tools::sendMessage( this->group, message );
    this->chatView->appendMessage( message );
}

void GroupPane::showEvent( QShowEvent* /* event */ )
{
    this->insertMessages();
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
                this->offlineContacts->addChild( twi );
            }
            else
            {
                this->onlineContacts->addChild( twi );
            }
        }
        this->onlineContacts->sortChildren( 0, Qt::AscendingOrder );
        this->offlineContacts->sortChildren( 0, Qt::AscendingOrder );

        this->onlineContacts->setText( 0, "Online (" + QString::number( this->onlineContacts->childCount() ) + ")" );
        this->offlineContacts->setText( 0, "Online (" + QString::number( this->offlineContacts->childCount() ) + ")" );

        this->groupContactsList->setUpdatesEnabled( true );
    }
}

void GroupPane::hideEvent( QHideEvent* /* event */ )
{
    this->chatView->insertMarker();
}
