#include "uitools.h"
#include "tools.h"
uiTools *uitools_;

uiTools::uiTools()
{
}

bool uiTools::checkChatWindowOpen( Target *target )
{
    return ( this->openWindows.value( target ) != NULL );
}

void uiTools::openChatWindow( Target *target )
{
    if ( this->checkChatWindowOpen( target ) )
    {
        ChatWindow *w = this->openWindows.value( target );
        w->raise();
        w->activateWindow();
    }
    else
    {
        ChatWindow *w = new ChatWindow( target );
        this->openWindows[ target ] = w;
        w->show();
        connect( w, SIGNAL(removeChatWindow(Target*)), this, SLOT(removeChatWindow(Target*)));
    }
}

void uiTools::removeChatWindow( Target *target )
{
    this->openWindows.remove( target );
}

void uiTools::showContactProperties( Contact *contact )
{
    ContactPropertiesWindow *c = ContactPropertiesWindow::getInstance();
    c->update( contact );
    c->show();
}
