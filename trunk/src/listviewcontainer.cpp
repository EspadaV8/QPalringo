/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QPalringo. If not, see <http://www.gnu.org/licenses/>       *
 *                                                                         *
 ***************************************************************************/
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
