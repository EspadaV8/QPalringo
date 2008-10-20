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
#include "tools.h"
#include "palringolistview.h"
#include "grouplistitem.h"

PalringoListView::PalringoListView( QWidget *parent, Group *group )
    : QScrollArea( parent )
{
    connect( tools_, SIGNAL( connected() ), this, SLOT( getContacts() ) );
    this->group = group;

    QWidget *w = new QWidget;
    w->setObjectName( "ListViewBackground" );

    listLayout = new QVBoxLayout;

    w->setLayout( listLayout );

    this->setWidgetResizable( true );
    this->setWidget( w );
}

void PalringoListView::setupOverview()
{
    this->listViewContainers.append( new ListViewContainer( this, "Services" ) );
    this->listViewContainers.append( new ListViewContainer( this, "Messages" ) );

    disconnect( tools_, SIGNAL( connected() ), this, SLOT( getContacts() ) );

    this->addLayoutsToSelf();
}

void PalringoListView::setupGroupList()
{
    this->listViewContainers.append( new ListViewContainer( this, "Group Chat" ) );

    GroupListItem *gli = new GroupListItem( this, this->group );
    this->addWidgetToView( gli );

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

bool PalringoListView::addWidgetToView( ListItem *item )
{
    QString containerGroup = item->getContainerGroup();
    int containerPosition = this->getContainerPosition( containerGroup );
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
            qDebug( "lvc is null - %s", qPrintable( item->getContainerGroup() ) );
            return false;
        }
    }
    else
    {
        qDebug( "invalid containerPosition - %d", containerPosition );
        return false;
    }
}

void PalringoListView::updateWidget( int x )
{
    ListItem *l = this->contactList.at( x );
    this->addWidgetToView( l );
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
        this->addWidgetToView( l );
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
        ContactListItem *pc = new ContactListItem( this, contact );
        this->contactList.append( pc );
        this->addWidgetToView( pc );
    }
}

void PalringoListView::getContacts()
{
    unsigned long long groupID = ( this->group == NULL ) ? 0 : this->group->getID();
    QHash<unsigned long long, Contact*> contacts = tools_->getContacts( groupID );

    if( contacts.size() > 0 )
    {
        foreach( Contact *contact, contacts )
        {
            this->contactReceived( contact );
        }
    }
}

PalringoListView::~PalringoListView()
{
}
