#include "overviewpane.h"

OverviewPane::OverviewPane( QWidget* parent )
    : QPalPane( parent )
{
    this->layout = new QHBoxLayout( this );
    this->statusView = new ChatView( this, "QPalringo Overview" );

    this->layout->addWidget( this->statusView );

    connect( this->statusView, SIGNAL(sendMessage(QString)), this, SLOT(addText(QString)));
}

QString OverviewPane::getTitle() const
{
    return "Palringo Overview";
}

void OverviewPane::addText( QString log )
{
    this->statusView->addText( log );
}
