//
// C++ Implementation: listitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listitem.h"

ListItem::ListItem( QWidget *parent )
    : QWidget( parent )
{
    ReloadStyleSheet();

    this->parent = parent;

    QHBoxLayout *hbox = new QHBoxLayout( this );

    this->svgIcon = new QSvgWidget();
    this->svgIcon->setMinimumSize( 24, 24 );
    this->svgIcon->setMaximumSize( 24, 24 );

    QVBoxLayout *vbox = new QVBoxLayout();

    this->firstLine = new QLabel( "" );
    this->firstLine->setObjectName( "firstLine" );

    this->secondLine = new QLabel( "" );
    this->secondLine->setObjectName( "secondLine" );

    vbox->addWidget( this->firstLine );
    vbox->addWidget( this->secondLine );

    hbox->addWidget( this->svgIcon );
    hbox->addLayout( vbox, 1 );

    setAutoFillBackground( false );
    setLayout( hbox );

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

ListItem::~ListItem() { }
