/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith   *
 *   espadav8@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "palringocontact.h"
#include "tools.h"

PalringoContact::PalringoContact( QWidget *parent, Contact *contact )
    : ListItem( parent )
{
    this->contact = contact;
    this->openChatWindow = false;
    this->chatWindow = NULL;

    this->setFirstLine( this->contact->getNickname() );
    this->setSecondLine( this->contact->getStatusline() );

    this->setIcon( this->getContactIcon() );

    connect( contact, SIGNAL( updateNickname( QString ) ), this, SLOT( setFirstLine( QString ) ) );
}

void PalringoContact::setContactNickname( QString nickname )
{
    this->contact->setNickname( nickname );
}

void PalringoContact::setContactStatusline( QString statusline )
{
    this->contact->setStatusline( statusline );
    // this->setSecondLine( this->contact->Statusline  );
}

void PalringoContact::setContactOnlineStatus( int onlinestatus )
{
    this->contact->setOnlineStatus( onlinestatus );

    //this->setSecondLine( this->contact->Statusline );
    //this->setIcon( this->getContactIcon() );
}

Contact* PalringoContact::getContact()
{
    return this->contact;
}

int PalringoContact::getContactOnlineStatus()
{
    return this->contact->getOnlineStatus();
}

QString PalringoContact::getContainerGroup()
{
    if( this->contact->getOnlineStatus() == 0 )
    {
        return "Offline";
    }
    else
    {
        return "Online";
    }
}

void PalringoContact::mouseDoubleClickEvent( QMouseEvent *event )
{
    // qDebug( "Double click on contact" );
    event->accept();
    tools_->openChatWindow( this->getContact() );
}

void PalringoContact::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PalringoContact::CreateChatWindow()
{
    this->chatWindow = new QWidget( this, Qt::Window );
}

QString PalringoContact::getContactIcon()
{
    QString iconName;
    if ( this->contact->getOnlineStatus() == 0 )
    {
        iconName = ":/contacts/offlineContactIcon.png";
    }
    else
    {
        switch( this->contact->getDeviceType() )
        {
            case 1:
                iconName = ":/contacts/botContactIcon.png";
                break;
            case 3:
                iconName = ":/contacts/mobileContactIcon.png";
                break;
            default:
            case 2:
                iconName = ":/contacts/pcContactIcon.png";
                break;
        }
    }
    return iconName;
}

PalringoContact::~PalringoContact() { }
