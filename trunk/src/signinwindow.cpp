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
#include <QSettings>
#include "signinwindow.h"
#include "tools.h"

SigninWindow::SigninWindow(QWidget *parent)
 : QDialog(parent), QPalringoSigninWindow()
{
    setupUi(this);
    this->setFeildValues();

    connect( this->OkButton, SIGNAL(clicked()), this, SLOT(openPalringoConnection()));
    connect( this->OkButton, SIGNAL(clicked()), this, SLOT(close()));
    connect( this->CancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void SigninWindow::setFeildValues()
{
    QSettings settings;
    
    this->EmailEdit->setText( settings.value( "signin/emailaddress" ).toString() );
    this->PasswordEdit->setText( settings.value( "signin/password" ).toString() );
    this->RememberMe->setChecked( settings.value( "signin/rememberme" ).toBool() );
    this->AutoSignin->setChecked( settings.value( "signin/autosignin" ).toBool() );
}

QString SigninWindow::getEmailAddress()
{
    return this->EmailEdit->text();
}

QString SigninWindow::getPassword()
{
    return this->PasswordEdit->text();
}

void SigninWindow::openPalringoConnection()
{
    if( this->RememberMe->isChecked() )
    {
        QSettings settings;
        
        settings.setValue( "signin/emailaddress", this->EmailEdit->text() );
        settings.setValue( "signin/password", this->PasswordEdit->text() );
        settings.setValue( "signin/rememberme", this->RememberMe->isChecked() );
        settings.setValue( "signin/autosignin", this->AutoSignin->isChecked() );
    }
    tools_->openPalringoConnection( this->getEmailAddress(), this->getPassword() );
}

SigninWindow::~SigninWindow() { }
