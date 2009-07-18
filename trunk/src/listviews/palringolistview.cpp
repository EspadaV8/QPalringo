/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as         *
 *  published by the Free Software Foundation, either version 3 of the     *
 *  License, or (at your option) any later version.                        *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU Lesser General Public License for more details.                    *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with QPalringo. If not, see                              *
 *  <http://www.gnu.org/licenses/>                                         *
 *                                                                         *
 ***************************************************************************/
#include "tools.h"
#include "palringolistview.h"
#include "defaultlistviewcontainer.h"

PalringoListView::PalringoListView( QWidget *parent )
    : QScrollArea( parent )
{
    QWidget *w = new QWidget;
    w->setObjectName( "ListViewBackground" );

    listLayout = new QVBoxLayout;
    listLayout->setContentsMargins( 0, 0, 0, 0 );
    listLayout->setSpacing( 0 );

    w->setLayout( listLayout );

    this->setWidgetResizable( true );
    this->setWidget( w );
}

void PalringoListView::inFocus() { }

void PalringoListView::addContainer( QString containerName )
{
    this->listViewContainers.append( new DefaultListViewContainer( this, containerName ) );
}

void PalringoListView::setupContainers()
{
    qDebug( "PalringoListView::setupContainers - not implemented" );
}

void PalringoListView::addLayoutsToSelf()
{
    foreach ( ListViewContainer *l, this->listViewContainers )
        this->listLayout->addWidget( l );

    this->listLayout->addStretch( 1 );
}

bool PalringoListView::addWidgetToView( ListItem *item )
{
    QString containerName = item->getContainerGroup();
    return this->addWidgetToView( item, containerName );
}

bool PalringoListView::addWidgetToView( ListItem *item, QString containerName )
{
    int containerPosition = this->getContainerPosition( containerName );
    if( containerPosition >= 0 )
    {
        ListViewContainer *lvc = this->listViewContainers.at( containerPosition );
        if ( lvc != NULL )
        {
            lvc->appendWidget( item );
            return true;
        }
        else
        {
            qDebug( "lvc is null - %s", qPrintable( containerName ) );
            return false;
        }
    }
    else
    {
        qDebug( "invalid containerPosition - %d", containerPosition );
        return false;
    }
}

void PalringoListView::removeListItem( ListItem *item )
{
    QString containerName = item->getContainerGroup();
    item->hide();
}

int PalringoListView::getContainerPosition( QString containerName )
{
    for( int i = 0; i < this->listViewContainers.size(); i++ )
    {
        ListViewContainer *lvc = this->listViewContainers.at( i );
        if ( lvc->getName() == containerName )
        {
            return i;
        }
    }
    return -1;
}

void PalringoListView::mousePressEvent( QMouseEvent *event )
{
    event->accept();
    // if we get a mouse event then check the contacts to see which one wants to be selected
    for( int i = 0; i < this->listItems.size(); i++ )
    {
        ListItem *l = this->listItems.at( i );
        l->setSelected( l->getToSelect() );
    }
}

PalringoListView::~PalringoListView()
{
}
