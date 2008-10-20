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
#include <QtGui>
#include "palringowindow.h"
#include "tools.h"
#include "palringoservice.h"
#include "serviceitem.h"

PalringoWindow::PalringoWindow()
 : QMainWindow()
{
    tools_ = new Tools( this );
    CreateTrayIcon();
    SetupActions();
    CreateMenuBar();

    SetupTabs();
    AddPalringoService();

    setCentralWidget( mainTabs );
    setWindowTitle( tr( "QPalringo" ) );
    setWindowIcon( *new QIcon( ":/svg/logo.svg" ) );

    connect( tools_, SIGNAL( newGroupAdded( Group* )), this, SLOT( newGroupAdded( Group* ) ) );
}

void PalringoWindow::SetupActions()
{
    onlineMenuAction = new QAction( tr( "&Online" ), this );
    onlineMenuAction->setShortcut( tr( "Ctrl+O" ) );
    onlineMenuAction->setStatusTip( tr( "Go online" ) );
    // connect(onlineMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    awayMenuAction = new QAction( tr( "&Away" ), this );
    awayMenuAction->setShortcut( tr( "Ctrl+A" ) );
    awayMenuAction->setStatusTip( tr( "Mark yourself as away" ) );
    // connect(awayMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    busyMenuAction = new QAction( tr( "&Busy" ), this );
    busyMenuAction->setShortcut( tr( "Ctrl+O" ) );
    busyMenuAction->setStatusTip( tr( "Mark yourself as busy" ) );
    // connect(busyMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    invisibleMenuAction = new QAction( tr( "&Invisible" ), this );
    invisibleMenuAction->setShortcut( tr( "Ctrl+I" ) );
    invisibleMenuAction->setStatusTip( tr( "Go invisible" ) );
    // connect(invisibleMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    offlineMenuAction = new QAction( tr( "Offlin&e" ), this );
    offlineMenuAction->setShortcut( tr( "Ctrl+E" ) );
    offlineMenuAction->setStatusTip( tr( "Go offline" ) );
    // connect(offlineMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    signinMenuAction = new QAction( tr( "Si&gn In" ), this );
    signinMenuAction->setShortcut( tr( "Ctrl+G" ) );
    signinMenuAction->setStatusTip( tr( "Sign In" ) );
    connect(signinMenuAction, SIGNAL(triggered()), this, SLOT(showSigninWindow()));

    addServiceMenuAction = new QAction( tr( "A&dd service..." ), this );
    addServiceMenuAction->setShortcut( tr( "Ctrl+D" ) );
    addServiceMenuAction->setStatusTip( tr( "Add service" ) );
    // connect(addServiceMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    settingsMenuAction = new QAction( tr("&Settings..."), this );
    settingsMenuAction->setShortcut(tr("Ctrl+S"));
    settingsMenuAction->setStatusTip(tr("Settings"));
    // connect(settingsMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    exitMenuAction = new QAction( tr("&Exit"), this );
    exitMenuAction->setShortcut( tr( "Ctrl+Q" ) );
    exitMenuAction->setStatusTip( tr( "Exit Palringo" ) );
    connect(exitMenuAction, SIGNAL( triggered() ), this, SLOT( close() ) );
}

void PalringoWindow::CreateMenuBar()
{
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
        palringoSubmenu = fileMenu->addMenu( tr( "&Palringo" ) );
        palringoSubmenu->addAction( onlineMenuAction );
        palringoSubmenu->addSeparator();
        palringoSubmenu->addAction( awayMenuAction );
        palringoSubmenu->addAction( busyMenuAction );
        palringoSubmenu->addAction( invisibleMenuAction );
        palringoSubmenu->addSeparator();
        palringoSubmenu->addAction( offlineMenuAction );
        palringoSubmenu->addSeparator();
        palringoSubmenu->addAction( signinMenuAction );
    fileMenu->addAction( addServiceMenuAction );
    fileMenu->addSeparator();
    fileMenu->addAction( settingsMenuAction );
    fileMenu->addSeparator();
    fileMenu->addAction( exitMenuAction );
}

void PalringoWindow::SetupTabs()
{
    mainTabs = new QTabWidget();

    overviewList = new PalringoListView( mainTabs );
    overviewList->setupOverview();

    contactList = new PalringoListView( mainTabs );
    contactList->setupContactList();
    connect( tools_, SIGNAL( userContactReceived( Contact* ) ), contactList, SLOT( contactReceived( Contact* ) ) );

    QIcon *p = new QIcon( ":/svg/palringoService.svg" );

    mainTabs->addTab( overviewList, *p, tr( "Overview" ) );
    mainTabs->addTab( contactList, tr( "&Contacts" ) );
}

void PalringoWindow::AddPalringoService()
{
    PalringoService *s = new PalringoService;
    s->Nickname = QString("Palringo");
    s->Status = QString("Offline");
    s->OnlineStatus = "Offline";
    s->Type = "palringo";
    s->Group = "Services";
    
    ServiceItem *si = new ServiceItem( 0, s, true );
    connect( si, SIGNAL(doubleClick()), this, SLOT(loginService()));
    userServices.append( si );
    overviewList->setList( userServices );
    this->update();
}

PalringoWindow::~PalringoWindow()
{
}

void PalringoWindow::CreateTrayIcon()
{
    this->systraySvg = new QIcon( ":/svg/logo.svg" );
    this->systrayicon = new QSystemTrayIcon();
    this->systrayicon->setIcon( *this->systraySvg );
    this->systrayicon->show();
}

void PalringoWindow::showSigninWindow()
{
    if( this->signinWindow ) {};
    this->signinWindow = new SigninWindow( this );
    this->signinWindow->show();
}

void PalringoWindow::loginService()
{
    this->showSigninWindow();
}

void PalringoWindow::newGroupAdded( Group *group )
{
    PalringoListView *groupTab = new PalringoListView( mainTabs, group );
    groupTab->setupGroupList();
    connect( tools_, SIGNAL( contactDetailReceived( Contact* ) ), groupTab, SLOT( contactReceived( Contact* ) ) );
    mainTabs->addTab( groupTab, group->getName() );
}
