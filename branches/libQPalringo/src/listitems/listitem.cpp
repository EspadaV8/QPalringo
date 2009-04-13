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
#include "listitem.h"

ListItem::ListItem( QWidget *parent )
    : QWidget( parent )
{
    ReloadStyleSheet();

    this->type = ListItem::UNDEFINED;

    QHBoxLayout *hbox = new QHBoxLayout;

    this->svgIcon = new QSvgWidget;
    this->svgIcon->setFixedSize( 24, 24 );

    QVBoxLayout *vbox = new QVBoxLayout;

    this->firstLine = new QLabel;
    this->firstLine->setObjectName( "firstLine" );

    this->secondLine = new QLabel;
    this->secondLine->setObjectName( "secondLine" );

    vbox->addWidget( this->firstLine );
    vbox->addWidget( this->secondLine );

    hbox->addWidget( this->svgIcon );
    hbox->addLayout( vbox, 1 );

    setAutoFillBackground( false );
    setLayout( hbox );
    
    this->popupMenu = new QMenu;

    this->toSelect = false;
    this->selected = false;
}

void ListItem::setFirstLine( QString text )
{
    this->firstLine->setText( text );
}

void ListItem::setSecondLine( QString text )
{
    this->secondLine->setText( text );
}

void ListItem::setIcon( QString iconFilename )
{
    this->svgIcon->load( iconFilename );
}

QString ListItem::getFirstLine()
{
    return this->firstLine->text();
}

QString ListItem::getContainerGroup()
{
    return "Undefined";
}

void ListItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ListItem::mousePressEvent( QMouseEvent *event )
{
    this->toSelect = true;
    event->ignore();
}

void ListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    emit( doubleClick() );
    event->accept();
}

void ListItem::contextMenuEvent( QContextMenuEvent *event )
{
    if( !this->popupMenu->isEmpty() )
    {
        event->accept();
        this->popupMenu->popup( event->globalPos() );
    }
}

void ListItem::ReloadStyleSheet()
{
    QFile sheet ( ":/styles/ListItem.css" );

    if ( ! sheet.open ( QIODevice::ReadOnly ) )
    {
        qDebug( "failed to read the stylesheet resource: %s", qPrintable( sheet.fileName() ) );
    }
    else
    {
        this->setStyleSheet ( sheet.readAll() );
    }
}

void ListItem::setMenu()
{
    qDebug( "ListItem::setMenu - no menu" );
}

bool ListItem::getToSelect()
{
    return this->toSelect;
}

void ListItem::setSelected( bool b )
{
    this->toSelect = false;
    this->setProperty( "selected", b );
    ReloadStyleSheet();
}

void ListItem::removeSelf()
{
    emit( removeListItem( this ) );
}

int ListItem::getType()
{
    return this->type;
}

ListItem::~ListItem()
{
    delete this->svgIcon;
    delete this->firstLine;
    delete this->secondLine;
    delete this->popupMenu;
}
