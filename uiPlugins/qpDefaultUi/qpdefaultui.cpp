#include <QtGui>
#include "qpdefaultui.h"
#include "listviews/contactlistview.h"
#include "listviews/grouplistview.h"
#include "tools.h"

QWidget* QPDefaultUi::getCentralWidget()
{
    this->mainTabs = new QTabWidget();

    overviewList = new OverviewListView( mainTabs, tools() );
    overviewList->setupContainers();

    mainTabs->addTab( overviewList, Tools::getPixmap( ":/svg/palringoService.svg" ), tr( "Overview" ) );

    ContactListView* contactList = new ContactListView( mainTabs, tools() );
    contactList->setupContainers();

    mainTabs->addTab( contactList, Tools::getPixmap( ":/svg/onlineContact.svg" ), tr( "&Contacts" ) );
    /*
    connect( tools_, SIGNAL( groupLeft( quint64 ) ), this, SLOT( groupLeft( quint64 ) ) );
    connect( tools_, SIGNAL( cleanUp() ), this, SLOT( cleanUp() ) );
    */

    connect( tools(), SIGNAL( newGroupAdded( Group* )), this, SLOT( newGroupAdded( Group* ) ) );
    connect( tools(), SIGNAL( newPendingMessage( Target* ) ), this, SLOT( newPendingMessage( Target* ) ) );
    connect( overviewList, SIGNAL(signinPalringo(QString, QString )), tools(), SLOT( openPalringoConnection( QString, QString ) ));

    connect( this->mainTabs, SIGNAL( currentChanged( int ) ), this, SLOT( tabFocusChanged( int ) ) );

    //connect( overviewList, SIGNAL(focusChatWindow(Target*)), this, SLOT(focusChatWindow(Target*)));
    connect( contactList, SIGNAL(focusChatWindow(Target*)), this, SLOT(focusChatWindow(Target*)));

    return this->mainTabs;
}

QString QPDefaultUi::getName()
{
    return "Default plugin";
}

void QPDefaultUi::newGroupAdded( Group *group )
{
    GroupListView *groupTab = new GroupListView( mainTabs, tools(), group );
    groupTab->setupContainers();
    this->mainTabs->addTab( groupTab, Tools::getPixmap( ":/svg/group.svg" ), group->getName() );

    connect( groupTab, SIGNAL(focusChatWindow(Target*)), this, SLOT(focusChatWindow(Target*)));
}

void QPDefaultUi::tabFocusChanged( int tabIndex )
{
    PalringoListView *p = (PalringoListView*)this->mainTabs->widget(tabIndex);
    if( p )
    {
        emit p->inFocus();
    }
}

bool QPDefaultUi::checkChatWindowOpen( Target *target )
{
    return ( this->openWindows.value( target ) != NULL );
}

void QPDefaultUi::focusChatWindow( Target* target )
{
    if ( this->checkChatWindowOpen( target ) )
    {
        ChatWindow *w = this->openWindows.value( target );
        w->raise();
        w->activateWindow();
    }
    else
    {
        ChatWindow *w = new ChatWindow( this->mainTabs, target );
        this->openWindows[ target ] = w;
        w->show();

        connect( w, SIGNAL( sendMessage( ChatWindow*, Target*,Message ) ), this, SLOT( sendMessage( ChatWindow*, Target*,Message ) ) );
        connect( w, SIGNAL( chatWindowClosed( Target* ) ), this, SLOT( removeChatWindow( Target* ) ) );
    }
}

void QPDefaultUi::removeChatWindow( Target *target )
{
    this->openWindows.remove( target );
}

void QPDefaultUi::newPendingMessage( Target* target )
{
    QSettings settings;

    if( !this->checkChatWindowOpen( target ) )
    {
        if( ( target->getType() == Target::CONTACT && settings.value( "alerts/privateAutoOpen" ).toBool() ) ||
            ( target->getType() == Target::GROUP && settings.value( "alerts/groupAutoOpen" ).toBool() ) )
        {
            this->focusChatWindow( target );
        }
        else
        {
            this->overviewList->newPendingMessage( target );
        }
    }
}

void QPDefaultUi::sendMessage( ChatWindow* chatwindow, Target* target, Message message )
{
    if( message.senderID() == 0 )
    {
        quint64 userId = tools()->getUser().userID;
        message.setSenderID( userId );
        message.setSender( tools()->getContact( userId ) );
    }
    tools()->sendMessage( target, message );
    if( true ) // TODO: Tools::sendMessage() should return a bool so we can tell if it's been sent or not
    {
        chatwindow->appendMessage( message );
    }
}

Q_EXPORT_PLUGIN2(qpdefaultui, QPDefaultUi)
