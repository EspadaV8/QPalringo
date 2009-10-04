#include "defaultlistviewcontainer.h"

DefaultListViewContainer::DefaultListViewContainer( QWidget* parent, QString name )
    : ListViewContainer( parent, name )
{
    outerLayout = new QVBoxLayout;
    outerLayout->setContentsMargins( 0, 0, 0, 0 );
    outerLayout->setSpacing( 0 );

    innerLayout = new QVBoxLayout;
    innerLayout->setContentsMargins( 0, 0, 0, 0 );
    innerLayout->setSpacing( 0 );

    layoutContainer = new QWidget;
    layoutContainer->setLayout( innerLayout );

    button = new QPushButton( this->downarrow, this->name );
    button->setObjectName( "listContainerToggleButton" );
    connect( button, SIGNAL( clicked() ), this, SLOT( buttonClicked() ) );

    outerLayout->addWidget( button );
    outerLayout->addWidget( layoutContainer );

    this->setLayout( outerLayout );
}

void DefaultListViewContainer::buttonClicked()
{
    if ( this->layoutContainer->isVisible() )
    {
        this->layoutContainer->setVisible( false );
        this->button->setIcon( this->rightarrow );
        this->button->setText( this->name + " (" + QString::number( innerLayout->count() ) + ")" );
    }
    else
    {
        this->layoutContainer->setVisible( true );
        this->button->setIcon( this->downarrow );
        this->button->setText( this->name );
    }
}

void DefaultListViewContainer::appendWidget( ListItem *widget, bool sorted )
{
    if( !sorted || ( this->innerLayout->count() == 0 ) )
    {
        innerLayout->addWidget( widget );
    }
    else
    {
        int i = 0;
        for( i = 0; i < innerLayout->count(); ++i )
        {
            ListItem *li = (ListItem*)innerLayout->itemAt( i )->widget();
            if( li->getFirstLine().toLower() < widget->getFirstLine().toLower() )
            {
                continue;
            }
            else
            {
                break;
            }
        }
        innerLayout->insertWidget( i, widget );
    }
}

int DefaultListViewContainer::hasWidget( ListItem *widget )
{
    return innerLayout->indexOf( widget );
}

void DefaultListViewContainer::removeWidget( ListItem *widget )
{
    innerLayout->removeWidget( widget );
}
