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
    this->setWindowIcon( Tools::getPixmap( tools_->getTargetIcon( contact ) ) );
    this->setWindowTitle( contact->getNickname() );
    this->nicknameLabel->setText( contact->getNickname() );
    this->statuslineLabel->setText( contact->getStatusline() );
    this->idLabel->setText( QString::number( contact->getID() ) );
}

ContactPropertiesWindow::~ContactPropertiesWindow() { }
