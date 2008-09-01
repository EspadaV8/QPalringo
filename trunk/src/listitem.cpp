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
    : QWidget( parent ), Ui::ListItem()
{
    setupUi( this );
    ReloadStyleSheet();

    this->parent = parent;
    this->icon = new QPixmap();

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
    this->icon->load( iconFilename );
    if ( ( this->icon->width() != MAX_ICON_SIZE ) || ( this->icon->height() != MAX_ICON_SIZE ) )
    {
        *this->icon = this->icon->scaled( MAX_ICON_SIZE, MAX_ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    }
    this->iconLabel->setPixmap( *this->icon );
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
    //qDebug( "toSelect set" );
    this->toSelect = true;
    event->ignore();
}

void ListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    //qDebug( "Double click" );
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
