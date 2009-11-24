#include "chatview.h"
#include "qptools.h"
#include "tools.h"
#include <QDebug>

ChatView::ChatView( QWidget* parent, QString title, QString iconFilename )
    : QWidget( parent )
{
    this->layout = new QVBoxLayout( this );
    this->layout->setSpacing( 0 );
    this->layout->setContentsMargins( 0, 0, 0, 0 );
/*
    this->chatview = new QTextBrowser();
    this->chatview->setReadOnly( true );
    this->chatview->setTextInteractionFlags( Qt::TextBrowserInteraction );
    this->chatview->setOpenExternalLinks( true );
*/
    this->messageList = new MessageList( this );

    this->titleLayout = new QHBoxLayout;
    this->titleLayout->setSpacing( 0 );
    this->titleLayout->setContentsMargins( 0, 0, 0, 0 );

    this->title = new QLabel;
    this->icon = new QLabel;

    this->setTitle( title );
    this->setIcon( iconFilename );

    this->titleLayout->addWidget( this->icon );
    this->titleLayout->addWidget( this->title );

    this->textInput = new QLineEdit;

    this->layout->addLayout( this->titleLayout );
//    this->layout->addWidget( this->chatview );
    this->layout->addWidget( this->messageList );
    this->layout->addWidget( this->textInput );

    this->marker = true;

    connect( this->textInput, SIGNAL( returnPressed () ), this, SLOT( sendText() ) );
}

void ChatView::addText( QString text )
{
    //this->chatview->append( text );
    //this->marker = false;
}

void ChatView::insertMarker()
{
    return;
    if( this->marker == false )
    {
        this->addText( "-------------------------" );
        this->marker = true;
    }
}

void ChatView::sendText()
{
    emit sendMessage( this->textInput->text() );
    this->textInput->clear();
}

void ChatView::setTitle( QString title )
{
    this->title->setText( title );
}
void ChatView::setIcon( QString iconFilename )
{
    if( !iconFilename.isEmpty() )
    {
        QPixmap p = QPTools::getPixmap( iconFilename, 20 );
        this->icon->setPixmap( p );
        this->icon->setFixedSize( p.width(), p.height() );
    }
}

void ChatView::appendMessage( Message message )
{
    /*
    QString colour;
    if( message.senderID() == Tools::getUser().userID )
    {
        colour = "red";
    }
    else
    {
        colour = "blue";
    }
    QString m;
    QString timestamp = QPTools::getMessageTimestamp( message ).toString( "hh:mm:ss" );
    m.append( "[" + timestamp + "] " );
    m.append( "<span style=\"color:" + colour + "\">" + message.sender()->getTitle() + "</span>" );
    m.append( ": " );
    if( message.type() == "text/plain" )
    {
        m.append( QPTools::formatMessageText( message.payload() ) );
    }
    else
    {
        m.append( message.type() );
    }

    this->addText( m );
    */
    this->messageList->addMessage( message );
}
