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
#include <QSettings>
#include "signinwindow.h"
#include "tools.h"

SigninWindow::SigninWindow(QWidget *parent)
 : QDialog(parent), QPalringoSigninWindow()
{
    setupUi(this);
    this->setFeildValues();
    this->AutoSignin->setEnabled( false );

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
    // this->AutoSignin->setChecked( settings.value( "signin/autosignin" ).toBool() );
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
        // settings.setValue( "signin/autosignin", this->AutoSignin->isChecked() );
    }
    tools_->openPalringoConnection( this->getEmailAddress(), this->getPassword() );
}

SigninWindow::~SigninWindow() { }
