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
#include "listviewcontainer.h"

ListViewContainer::ListViewContainer( QWidget *parent, QString name )
 : QWidget(parent)
{
    this->name = name;
    this->downarrow = new QPixmap( ":/misc/downarrow.png" );
    this->rightarrow = new QPixmap( ":/misc/rightarrow.png" );

    outerLayout = new QVBoxLayout;
    innerLayout = new QVBoxLayout;

    layoutContainer = new QWidget;
    layoutContainer->setLayout( innerLayout );

    button = new QPushButton( *downarrow, this->name );
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
        this->button->setIcon( *this->rightarrow );
    }
    else
    {
        this->layoutContainer->setVisible( true );
        this->button->setIcon( *this->downarrow );
    }
}

void ListViewContainer::appendWidget( ListItem *widget )
{
    innerLayout->addWidget( widget );
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


