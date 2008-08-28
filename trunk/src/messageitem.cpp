//
// C++ Implementation: messageitem
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "messageitem.h"
#include "contact.h"
#include "tools.h"

MessageItem::MessageItem( QWidget *parent, Message *message ) :
        QWidget( parent )
{
    Contact* contact = tools_->getContact( message->senderID );

    this->message = message;

    this->messageIcon = new QLabel( "" );
    this->messageText = new QLabel( "" );
    this->messageText->setWordWrap( true );
    this->messageText->setTextFormat( Qt::RichText );
    this->messageText->setTextInteractionFlags( Qt::TextBrowserInteraction );
    this->messageText->setOpenExternalLinks( true );

    this->sender = new QLabel( contact->getNickname() );
    this->sender->setAlignment( Qt::AlignLeft );

    this->timestamp = new QLabel( this->message->timestamp );
    this->timestamp->setAlignment( Qt::AlignRight );

    QString messageTypeIcon;
    if( this->message->type == "text/plain" )
    {
        QString formattedText = tools_->formatMessageText( this->message->payload  );
        this->messageText->setText( formattedText );
        messageTypeIcon = ":/messageTypes/text.png";
    }
    else if( this->message->type.startsWith( "image" ) )
    {
        QImage* im = new QImage();
        im->loadFromData( this->message->payload );
        QImage scaled = im->scaled( 100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation );

        QPixmap pi = QPixmap::fromImage( scaled );
        this->messageText->setPixmap( pi );
        messageTypeIcon = ":/messageTypes/image.png";
    }
    else if( this->message->type.startsWith( "audio" ) )
    {
        this->messageText->setText( "Audio not supported" );
        messageTypeIcon = ":/messageTypes/voice.png";
    }

    QPixmap *p = new QPixmap( messageTypeIcon );
    this->messageIcon->setPixmap( *p );
    this->layout = new QHBoxLayout();

    this->leftside = new QVBoxLayout();
    this->rightside = new QVBoxLayout();
    this->headers = new QHBoxLayout();

    this->leftside->addWidget( this->messageIcon );
    this->leftside->addStretch( 1 );

    this->headers->addWidget( this->sender );
    this->headers->addWidget( this->timestamp );

    this->rightside->addLayout( this->headers );
    this->rightside->addWidget( this->messageText );
    this->rightside->addStretch( 1 );

    this->layout->addLayout( this->leftside );
    this->layout->addLayout( this->rightside, 1 );
    this->setLayout( layout );
    ReloadStyleSheet();
}


MessageItem::~MessageItem()
{
}


void MessageItem::mousePressEvent( QMouseEvent *event )
{
    this->toSelect = true;
    event->ignore();
}

void MessageItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    emit( doubleClick() );
    event->accept();
}

bool MessageItem::getToSelect()
{
    return this->toSelect;
}

void MessageItem::setSelected( bool b )
{
    this->toSelect = false;
    this->setProperty( "selected", b );
    ReloadStyleSheet();
}

void MessageItem::ReloadStyleSheet()
{
    QFile sheet ( ":/styles/MessageItem.css" );

    if ( ! sheet.open ( QIODevice::ReadOnly ) )
    {
        qDebug( "failed to read the stylesheet resource: %s", qPrintable( sheet.fileName() ) );
    }
    else
    {
        this->setStyleSheet ( sheet.readAll() );
    }
}

void MessageItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
