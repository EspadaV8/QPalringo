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
#include <QIntValidator>
#include "settingswindow.h"
#include "tools.h"

SettingsWindow::SettingsWindow(QWidget *parent)
 : QDialog(parent), QPalringoSettingsWindow()
{
    setupUi(this);

    this->setFeildValues();

    connect( this->OkButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect( this->OkButton, SIGNAL(clicked()), this, SLOT(close()));
    connect( this->CancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void SettingsWindow::setFeildValues()
{
    QSettings settings;

    this->RememberMe->setChecked( settings.value( "signin/rememberme" ).toBool() );
    this->privateAutoOpen->setChecked( settings.value( "alerts/privateAutoOpen" ).toBool() );
    this->groupAutoOpen->setChecked( settings.value( "alerts/groupAutoOpen" ).toBool() );

    this->guiLayoutComboBox->setCurrentIndex( settings.value( "gui/layout" ).toInt() );
    this->showMessageIcons->setChecked( settings.value( "gui/showMessageIcons" ).toBool() );

    this->proxyEnable->setChecked( settings.value( "networt/proxy/enabled" ).toBool() );
    this->proxyTypeComboBox->setCurrentIndex( settings.value( "networt/proxy/proxytype" ).toInt() );
    this->proxyHostEdit->setText( settings.value( "networt/proxy/host" ).toString() );
    this->proxyPortEdit->setText( settings.value( "networt/proxy/port" ).toString() );
    this->proxyUsernameEdit->setText( settings.value( "networt/proxy/username" ).toString() );
    this->proxyPasswordEdit->setText( settings.value( "networt/proxy/password" ).toString() );

    this->serverHostEdit->setText( settings.value( "server/host", "primary.palringo.com" ).toString() );
    this->portSpinBox->setValue( settings.value( "server/port", 443 ).toInt() );
}

void SettingsWindow::saveSettings()
{
    QSettings settings;

    settings.setValue( "signin/rememberme", this->RememberMe->isChecked() );
    settings.setValue( "alerts/privateAutoOpen", this->privateAutoOpen->isChecked() );
    settings.setValue( "alerts/groupAutoOpen", this->groupAutoOpen->isChecked() );

    settings.setValue( "gui/layout", this->guiLayoutComboBox->currentIndex() );
    settings.setValue( "gui/showMessageIcons", this->showMessageIcons->isChecked() );

    settings.setValue( "networt/proxy/enabled", this->proxyEnable->isChecked() );
    settings.setValue( "networt/proxy/proxytype", this->proxyTypeComboBox->currentIndex() );
    settings.setValue( "networt/proxy/host", this->proxyHostEdit->text() );
    settings.setValue( "networt/proxy/port", this->proxyPortEdit->text() );
    settings.setValue( "networt/proxy/username", this->proxyUsernameEdit->text() );
    settings.setValue( "networt/proxy/password", this->proxyPasswordEdit->text() );

    settings.setValue( "server/host", this->serverHostEdit->text() );
    settings.setValue( "server/port", this->portSpinBox->value() );
}

SettingsWindow::~SettingsWindow() { }
