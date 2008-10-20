//
// C++ Implementation: listviewcontainer
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tools.h"
#include "listviewcontainer.h"

ListViewContainer::ListViewContainer( QWidget *parent, QString name )
 : QWidget(parent)
{
    this->name = name;
    this->downarrow = tools_->getPixmap( ":/misc/downarrow.png" );
    this->rightarrow = tools_->getPixmap( ":/misc/rightarrow.png" );

    outerLayout = new QVBoxLayout;
    innerLayout = new QVBoxLayout;

    layoutContainer = new QWidget;
    layoutContainer->setLayout( innerLayout );

    button = new QPushButton( this->downarrow, this->name );
    button->setObjectName( "listContainerToggleButton" );
    connect( button, SIGNAL( clicked() ), this, SLOT( buttonClicked() ) );

    outerLayout->addWidget( button );
    outerLayout->addWidget( layoutContainer );

    this->setLayout( outerLayout );
}

void ListViewContainer::buttonClicked()
{
    if ( this->layoutContainer->isVisible() )
    {
        this->layoutContainer->setVisible( false );
        this->button->setIcon( this->rightarrow );
    }
    else
    {
        this->layoutContainer->setVisible( true );
        this->button->setIcon( this->downarrow );
    }
}

void ListViewContainer::appendWidget( ListItem *widget, bool sorted )
{
    if( ( !sorted ) || ( innerLayout->count() == 0 ) )
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

void ListViewContainer::removeWidget( ListItem *widget )
{
    innerLayout->removeWidget( widget );
}

QString ListViewContainer::getName()
{
    return this->name;
}

void ListViewContainer::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ListViewContainer::~ListViewContainer()
{
}
