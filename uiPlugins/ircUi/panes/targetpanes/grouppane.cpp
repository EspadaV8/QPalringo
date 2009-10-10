#include "grouppane.h"
#include "qptools.h"
#include "tools.h"

GroupPane::GroupPane( Group* group, QWidget* parent )
    : TargetPane( group, parent )
{
    this->group = group;

    this->layout = new QVBoxLayout( this );
    this->splitter = new QSplitter();
    this->groupContactsList = new QListWidget();

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
        this->groupContacts = Tools::getGroupContacts( this->group->getID() );
        foreach( Contact* user, this->groupContacts )
        {
            QPixmap p = QPTools::getPixmap( QPTools::getTargetIcon( user ) );
            new QListWidgetItem( p, user->getNickname(), this->groupContactsList );
        }
        this->groupContactsList->sortItems( Qt::AscendingOrder );
    }
}

void GroupPane::hideEvent( QHideEvent* /* event */ )
{
    this->chatView->insertMarker();
}
