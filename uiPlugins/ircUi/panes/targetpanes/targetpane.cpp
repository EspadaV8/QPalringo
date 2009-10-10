#include "targetpane.h"

TargetPane::TargetPane( Target* target, QWidget* parent )
    : QPalPane( parent )
{
    this->target = target;
    this->chatView = new ChatView( this );
}

QString TargetPane::getTitle() const
{
    return "TargetPane::getTitle - Unimplemented";
}

void TargetPane::addText( QString text )
{
    this->chatView->addText( text );
}
