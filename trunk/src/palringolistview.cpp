/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith   *
 *   espadav8@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "palringolistview.h"

PalringoListView::PalringoListView( QWidget *parent, Group *group )
    : QScrollArea( parent )
{
    this->group = group;

    QWidget *w = new QWidget;
    w->setObjectName( "ListViewBackground" );

    downarrow = new QPixmap( ":/downarrow.png" );
    rightarrow = new QPixmap( ":/rightarrow.png" );

    listLayout = new QVBoxLayout;

    w->setLayout( listLayout );

    this->setWidgetResizable( true );
    this->setWidget( w );
}

void PalringoListView::setupOverview()
{
    this->listViewContainers.append( new ListViewContainer( this, "Services" ) );
    this->listViewContainers.append( new ListViewContainer( this, "Messages" ) );

    this->addLayoutsToSelf();
}

void PalringoListView::setupGroupList()
{
    this->listViewContainers.append( new ListViewContainer( this, "Group Chat" ) );

    setupContactList();
}

void PalringoListView::setupContactList()
{
    this->listViewContainers.append( new ListViewContainer( this, "Online" ) );
    this->listViewContainers.append( new ListViewContainer( this, "Offline" ) );

    this->addLayoutsToSelf();
}

void PalringoListView::addLayoutsToSelf()
{
    foreach ( ListViewContainer *l, this->listViewContainers )
        this->listLayout->addWidget( l );

    this->listLayout->addStretch( 1 );
}

void PalringoListView::updateWidget( int x )
{
    ListItem *l = this->contactList.at( x );
    ListViewContainer *lvc = this->listViewContainers.at( getContainerPosition( l->getContainerGroup() ) );
    if ( lvc != NULL )
        lvc->appendWidget( l );
    else
        qDebug( "lvc is null - %s", qPrintable( l->getContainerGroup() ) );
}

void PalringoListView::setList( QList<ListItem *> list )
{
    this->contactList = list;

    // we now go through each item
    for ( int i = 0; i < this->contactList.size(); i++ )
    {
        // get the list item
        ListItem *l = this->contactList.at( i );
        // do something?
        ListViewContainer *lvc = this->listViewContainers.at( getContainerPosition( l->getContainerGroup() ) );
        if ( lvc != NULL )
            lvc->appendWidget( l );
        else
            qDebug( "lvc is null - %s", qPrintable( l->getContainerGroup() ) );
    }
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
    // qDebug( "got a mouse event" );
    event->accept();
    // if we get a mouse event then check the contacts to see which one wants to be selected
    for( int i = 0; i < this->contactList.size(); i++ )
    {
        ListItem *l = this->contactList.at( i );
        l->setSelected( l->getToSelect() );
    }
}

void PalringoListView::contactReceived( Contact *contact )
{
    if( ( this->group == NULL ) || ( this->group->hasContact( contact->getID() ) ) )
    {
        PalringoContact *pc = new PalringoContact( this, contact );
        this->contactList.append( pc );

        ListViewContainer *lvc = this->listViewContainers.at( getContainerPosition( pc->getContainerGroup() ) );
        if ( lvc != NULL )
            lvc->appendWidget( pc );
        else
            qDebug( "lvc is null - %s", qPrintable( pc->getContainerGroup() ) );
    }
}

PalringoListView::~PalringoListView()
{
}
