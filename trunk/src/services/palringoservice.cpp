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
#include "palringoservice.h"
#include "tools.h"

PalringoService::PalringoService()
 : Service()
{
    this->signinWindow = NULL;
    connect( tools_, SIGNAL( connected() ), this, SLOT( loggedOn() ) );
}

void PalringoService::loggedOn()
{
    this->nickname = tools_->getUser().nickname;
    this->status = tools_->getUser().status;
    this->onlineStatus = qpOnlineStatus::ONLINE;
    emit serviceUpdated();
}

void PalringoService::loginService()
{
    if( this->signinWindow == NULL )
    {
        this->signinWindow = new SigninWindow();
    }
    this->signinWindow->show();
}

void PalringoService::changeOnlineStatus()
{
    QObject* sender = QObject::sender();
    QAction* action = qobject_cast<QAction *>(sender);

    if( action != 0 )
    {
        QString status;
        if( action->text() == "Online" )
        {
            status = QString::number(qpOnlineStatus::ONLINE);
        }
        else if( action->text() == "Away" )
        {
            status = QString::number(qpOnlineStatus::AWAY);
        }
        else if( action->text() == "Invisible" )
        {
            status = QString::number(qpOnlineStatus::INVISIBLE);
        }
        else if( action->text() == "Busy" )
        {
            status = QString::number(qpOnlineStatus::BUSY);
        }

        if( tools_->isLoggedIn() )
        {
            tools_->updateContactDetail( "ONLINE-STATUS", status );
        }
        else
        {
            this->loginService();
        }
    }
}

PalringoService::~PalringoService()
{
}
