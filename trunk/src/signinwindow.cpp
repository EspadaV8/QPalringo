//
// C++ Implementation: signinwindow
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "signinwindow.h"
#include "tools.h"

SigninWindow::SigninWindow(QWidget *parent)
 : QDialog(parent), QPalringoSigninWindow()
{
    setupUi(this);

    connect( this->OkButton, SIGNAL(clicked()), this, SLOT(openPalringoConnection()));
    connect( this->OkButton, SIGNAL(clicked()), this, SLOT(close()));
    connect( this->CancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

QString SigninWindow::getEmailAddress()
{
    if( this->PasswordEdit->text() == "" )
    {
        return QString( "asmith@palringo.com" );
    }

    return this->EmailEdit->text();
}

QString SigninWindow::getPassword()
{
    if( this->PasswordEdit->text() == "" )
    {
        return QString( "123456" );
    }

    return this->PasswordEdit->text();
}

SigninWindow::~SigninWindow()
{
}

void SigninWindow::openPalringoConnection()
{
    tools_->openPalringoConnection( this->getEmailAddress(), this->getPassword() );
}
