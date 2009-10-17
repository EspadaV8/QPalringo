#include "privatechatpane.h"
#include "qptools.h"

PrivateChatPane::PrivateChatPane( Contact* contact, QWidget* parent )
    : TargetPane( contact, parent )
{
    this->contact = contact;

    this->layout = new QVBoxLayout( this );
    this->layout->setContentsMargins( 0, 0, 0, 0 );

    this->chatView->setTitle( this->contact->getTitle() );
    this->chatView->setIcon( QPTools::getTargetIcon( contact ) );

    this->layout->addWidget( this->chatView );
}
