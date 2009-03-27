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
#include <QtGui>
#include "palringowindow.h"
#include "tools.h"
#include "listviews/contactlistview.h"
#include "listviews/grouplistview.h"
#include "listviews/overviewlistview.h"

PalringoWindow::PalringoWindow()
 : QMainWindow()
{
    tools_ = new Tools( this );
    CreateTrayIcon();
    SetupActions();
    CreateMenuBar();

    SetupTabs();

    setCentralWidget( mainTabs );
    setWindowTitle( tr( "QPalringo" ) );
    setWindowIcon( tools_->getPixmap( ":/svg/logo.svg" ) );

    connect( tools_, SIGNAL( newGroupAdded( Group* )), this, SLOT( newGroupAdded( Group* ) ) );
    connect( tools_, SIGNAL( groupLeft( quint64 ) ), this, SLOT( groupLeft( quint64 ) ) );
}

void PalringoWindow::SetupActions()
{
    /**
     * File menu actions
     */
    onlineMenuAction = new QAction( tr( "&Online" ), this );
    onlineMenuAction->setShortcut( tr( "Ctrl+O" ) );
    onlineMenuAction->setStatusTip( tr( "Go online" ) );
    onlineMenuAction->setEnabled( false );
    // connect(onlineMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    awayMenuAction = new QAction( tr( "&Away" ), this );
    awayMenuAction->setShortcut( tr( "Ctrl+A" ) );
    awayMenuAction->setStatusTip( tr( "Mark yourself as away" ) );
    awayMenuAction->setEnabled( false );
    // connect(awayMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    busyMenuAction = new QAction( tr( "&Busy" ), this );
    busyMenuAction->setShortcut( tr( "Ctrl+O" ) );
    busyMenuAction->setStatusTip( tr( "Mark yourself as busy" ) );
    busyMenuAction->setEnabled( false );
    // connect(busyMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    invisibleMenuAction = new QAction( tr( "&Invisible" ), this );
    invisibleMenuAction->setShortcut( tr( "Ctrl+I" ) );
    invisibleMenuAction->setStatusTip( tr( "Go invisible" ) );
    invisibleMenuAction->setEnabled( false );
    // connect(invisibleMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    offlineMenuAction = new QAction( tr( "Offlin&e" ), this );
    offlineMenuAction->setShortcut( tr( "Ctrl+E" ) );
    offlineMenuAction->setStatusTip( tr( "Go offline" ) );
    offlineMenuAction->setEnabled( false );
    // connect(offlineMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    signinMenuAction = new QAction( tr( "Si&gn In" ), this );
    signinMenuAction->setShortcut( tr( "Ctrl+G" ) );
    signinMenuAction->setStatusTip( tr( "Sign In" ) );
    signinMenuAction->setEnabled( false );
    // connect(signinMenuAction, SIGNAL(triggered()), this, SLOT(showSigninWindow()));

    addServiceMenuAction = new QAction( tr( "A&dd service..." ), this );
    addServiceMenuAction->setShortcut( tr( "Ctrl+D" ) );
    addServiceMenuAction->setStatusTip( tr( "Add service" ) );
    addServiceMenuAction->setEnabled( false );
    // connect(addServiceMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    settingsMenuAction = new QAction( tr("&Settings..."), this );
    settingsMenuAction->setShortcut(tr("Ctrl+S"));
    settingsMenuAction->setStatusTip(tr("Settings"));
    settingsMenuAction->setEnabled( false );
    // connect(settingsMenuAction, SIGNAL(triggered()), this, SLOT(newFile()));

    exitMenuAction = new QAction( tr("&Exit"), this );
    exitMenuAction->setShortcut( tr( "Ctrl+Q" ) );
    exitMenuAction->setStatusTip( tr( "Exit Palringo" ) );
    connect(exitMenuAction, SIGNAL( triggered() ), this, SLOT( close() ) );
    
    /**
     * Group menu actions
     */
    joinGroup = new QAction( tr("&Join a group"), this );
    joinGroup->setShortcut(tr("Ctrl+J"));
    joinGroup->setStatusTip(tr("Join an existing group"));
    connect(joinGroup, SIGNAL(triggered()), this, SLOT(joinAGroup()));
    
    createGroup = new QAction( tr("&Create a group"), this );
    createGroup->setShortcut(tr("Ctrl+C"));
    createGroup->setStatusTip(tr("Create a new group"));
    connect(createGroup, SIGNAL(triggered()), this, SLOT(createAGroup()));
}

void PalringoWindow::CreateMenuBar()
{
    /**
     * File menu
     */
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
    
    /**
     * Group menu
     */
    groupMenu = menuBar()->addMenu( tr( "&Groups" ) );
    groupMenu->addAction( joinGroup );
    groupMenu->addAction( createGroup );
}

void PalringoWindow::SetupTabs()
{
    mainTabs = new QTabWidget();
    connect(mainTabs, SIGNAL(currentChanged(int)), this, SLOT(tabFocusChanged(int)));

    OverviewListView *overviewList = new OverviewListView( mainTabs );
    overviewList->setupContainers();

    ContactListView *contactList = new ContactListView( mainTabs );
    contactList->setupContainers();

    mainTabs->addTab( overviewList, tools_->getPixmap( ":/svg/palringoService.svg" ), tr( "Overview" ) );
    mainTabs->addTab( contactList, tools_->getPixmap( ":/svg/onlineContact.svg" ), tr( "&Contacts" ) );
}

void PalringoWindow::tabFocusChanged(int tabIndex )
{
    PalringoListView *p = (PalringoListView*)this->mainTabs->widget(tabIndex);
    emit( p->inFocus() );
}

PalringoWindow::~PalringoWindow()
{
}

void PalringoWindow::CreateTrayIcon()
{
    this->systrayicon = new QSystemTrayIcon();
    this->systrayicon->setIcon( tools_->getPixmap( ":/svg/logo.svg" ) );
    this->systrayicon->show();
}

void PalringoWindow::newGroupAdded( Group *group )
{
    GroupListView *groupTab = new GroupListView( mainTabs, group );
    groupTab->setupContainers();
    mainTabs->addTab( groupTab, tools_->getPixmap( ":/svg/group.svg" ), group->getName() );
}

void PalringoWindow::groupLeft( quint64 groupID __attribute__ ((unused)) )
{
    QWidget *w = mainTabs->currentWidget();
    mainTabs->setCurrentIndex( 0 );
    mainTabs->removeTab( mainTabs->indexOf( w ) );
    w->deleteLater();
}

void PalringoWindow::joinAGroup()
{
    bool ok;
    QString groupName = QInputDialog::getText(0,
                                              tr("Join a Group"),
                                              tr("Enter the name of a group to join:"),
                                              QLineEdit::Normal,
                                              "",
                                              &ok,
                                              0);
    
    if( ok && !groupName.isEmpty() )
    {
        tools_->joinGroup( groupName );
    }
}

void PalringoWindow::createAGroup()
{
    bool ok;
    QString groupName = QInputDialog::getText(0,
                                              tr("Create a Group"),
                                              tr("Enter the name of a group to create:"),
                                              QLineEdit::Normal,
                                              "",
                                              &ok,
                                              0);
    
    if( ok && !groupName.isEmpty() )
    {
        tools_->createGroup( groupName );
    }
}
