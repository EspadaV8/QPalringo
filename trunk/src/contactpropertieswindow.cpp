//
// C++ Implementation: contactpropertieswindow
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tools.h"
#include "contactpropertieswindow.h"

ContactPropertiesWindow* ContactPropertiesWindow::pinstance = 0;

ContactPropertiesWindow::ContactPropertiesWindow()
    : QDialog(0), ContactProperties()
{
    setupUi( this );
}

ContactPropertiesWindow* ContactPropertiesWindow::getInstance()
{
    if( pinstance == 0 )
    {
        pinstance = new ContactPropertiesWindow;
    }
    return pinstance;
}

void ContactPropertiesWindow::update( Contact *contact )
{
    this->setWindowIcon( tools_->getPixmap( contact->getIcon() ) );
    this->setWindowTitle( contact->getNickname() );
    this->nicknameLabel->setText( contact->getNickname() );
    this->statuslineLabel->setText( contact->getStatusline() );
    this->idLabel->setText( QString::number( contact->getID() ) );
}

ContactPropertiesWindow::~ContactPropertiesWindow() { }
