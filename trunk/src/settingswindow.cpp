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
}

void SettingsWindow::saveSettings()
{
    QSettings settings;

    settings.setValue( "signin/rememberme", this->RememberMe->isChecked() );
    settings.setValue( "alerts/privateAutoOpen", this->privateAutoOpen->isChecked() );
    settings.setValue( "alerts/groupAutoOpen", this->groupAutoOpen->isChecked() );
}

SettingsWindow::~SettingsWindow() { }
