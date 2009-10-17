#include "targetpane.h"
#include "tools.h"
#include "qptools.h"

TargetPane::TargetPane( Target* target, QWidget* parent )
    : QPalPane( parent )
{
    this->target = target;
    this->chatView = new ChatView( this );

    connect( this->target, SIGNAL( pendingMessage( Target* ) ), this, SLOT( handleNewMessage( Target* ) ) );
    connect( this->chatView, SIGNAL(sendMessage(QString)), this, SLOT( sendTextMessage( QString ) ) );
}

QString TargetPane::getTitle() const
{
    return "TargetPane::getTitle - Unimplemented";
}

void TargetPane::addText( QString text )
{
    this->chatView->addText( text );
}

void TargetPane::appendMessage( Message message )
{
    this->chatView->appendMessage( message );
}

void TargetPane::handleNewMessage( Target* /* target */ )
{
    if( this->isHidden() )
    {
        emit updateTargetIcon( this->target );
    }
    else
    {
        this->insertMessages();
    }
}

void TargetPane::insertMessages()
{
    foreach( Message message, this->target->getPendingMessages() )
    {
        this->chatView->appendMessage( message );
    }
    this->target->clearPending();

    emit updateTargetIcon( this->target );
}

void TargetPane::sendTextMessage( QString text )
{
    Message message = QPTools::createTextMessage( text );
    if( message.senderID() == 0 )
    {
        quint64 userId = Tools::getUser().userID;
        message.setSenderID( userId );
        message.setSender( Tools::getContact( userId ) );
    }
    Tools::sendMessage( this->target, message );
    this->chatView->appendMessage( message );
}

void TargetPane::showEvent( QShowEvent* /* event */ )
{
    this->insertMessages();
}

void TargetPane::hideEvent( QHideEvent* /* event */ )
{
    this->chatView->insertMarker();
}
