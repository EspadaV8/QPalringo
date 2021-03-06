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
#include <QDebug>
#include "palringowindow.h"
#include "tools.h"
#include "qptools.h"
#include "uiplugininterface.h"

// Q_IMPORT_PLUGIN(qpdefaultui)

PalringoWindow::PalringoWindow()
 : QMainWindow()
{
    tools_ = new Tools( this );
    this->settingsWindow = NULL;
    setupUi();
    readSettings();
    loadUi();
    tools_->addInitialPalringoService();
}

void PalringoWindow::loadUi()
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        qDebug( "Trying to load plugin" );
        this->initUiPlugin( plugin );
    }

    QDir pluginsDir = QDir(qApp->applicationDirPath());

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader( pluginsDir.absoluteFilePath(fileName) );
        QObject *plugin = loader.instance();

        if( plugin )
        {
            qDebug( "Loaded - %s", qPrintable( fileName ) );
            this->initUiPlugin( plugin );
            break;
        }
        else
        {
            qDebug() << "Failed loading " << fileName << " - " << loader.errorString();
        }
    }
}

void PalringoWindow::initUiPlugin( QObject* plugin )
{
    UiPluginInterface* uiPlugin = qobject_cast<UiPluginInterface *>(plugin);
    if( uiPlugin )
    {
        qDebug() << uiPlugin->getName();
        uiPlugin->setTools( tools_ );
        uiPlugin->setup();
        QWidget *w = uiPlugin->getCentralWidget();
        setCentralWidget( w );
    }
}


void PalringoWindow::setupUi()
{
    CreateTrayIcon();
    SetupActions();
    CreateMenuBar();
/*
    QSettings settings;
    int uiLayout = settings.value( "gui/layout" ).toInt();

    switch( uiLayout )
    {
        case 1:
            setupButtonLayout();
            break;
    }
*/
    setWindowTitle( tr( "QPalringo" ) );
    setWindowIcon( QPTools::getPixmap( ":/svg/logo.svg" ) );
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
    //settingsMenuAction->setEnabled( false );
    connect(settingsMenuAction, SIGNAL(triggered()), this, SLOT(showSettingsWindow()));

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

void PalringoWindow::setupButtonLayout()
{
/*
    pages = new QStackedWidget;

    QWidget *buttons = new QWidget;
    buttons->setObjectName( "buttonsContainer" );
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setContentsMargins( 0, 0, 0, 0 );
    buttonsLayout->setSpacing( 0 );

    QPushButton *overviewButton = new QPushButton( tools_->getPixmap( ":/svg/logo.svg" ), "", this);
    overviewButton->setObjectName( "overviewButton" );
    //overviewButton->setCheckable(true);
    QPushButton *contactsButton = new QPushButton( tools_->getPixmap( ":/svg/onlineContact.svg" ), "&Contacts", this);
    contactsButton->setObjectName( "contactsButton" );
    //contactsButton->setCheckable(true);
    QPushButton *groupsButton = new QPushButton( tools_->getPixmap( ":/svg/group.svg" ), "&Groups", this);
    groupsButton->setObjectName( "groupsButton" );
    //groupsButton->setCheckable(true);

    connect( overviewButton, SIGNAL( clicked() ), this, SLOT( showOverview() ) );
    connect( contactsButton, SIGNAL( clicked() ), this, SLOT( showContacts() ) );

    QMenu *m = new QMenu;
    m->addAction( tools_->getPixmap( ":/svg/group.svg" ), "Palringo" );
    m->addAction( tools_->getPixmap( ":/svg/group.svg" ), "uk" );
    m->addAction( tools_->getPixmap( ":/svg/group.svg" ), "987654321" );

    groupsButton->setMenu( m );

    buttonsLayout->addWidget( overviewButton );
    buttonsLayout->addWidget( contactsButton );
    buttonsLayout->addWidget( groupsButton );
    buttonsLayout->addStretch(1);
    buttons->setLayout( buttonsLayout );

    OverviewListView *overviewList = new OverviewListView( this );
    overviewList->setupContainers();

    ContactListView *contactList = new ContactListView( this );
    contactList->setupContainers();

    pages->addWidget( overviewList );
    pages->addWidget( contactList );

    QWidget *w = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout;
    vb->setContentsMargins( 0, 0, 0, 0 );
    vb->setSpacing( 0 );

    vb->addWidget( buttons );
    vb->addWidget( pages );

    w->setLayout( vb );

    this->setCentralWidget( w );
*/
}

PalringoWindow::~PalringoWindow()
{
    delete this->settingsWindow;
}

void PalringoWindow::CreateTrayIcon()
{
    this->systrayicon = new QSystemTrayIcon();
    this->systrayicon->setIcon( QPTools::getPixmap( ":/svg/logo.svg" ) );
    this->systrayicon->show();

    connect( tools_, SIGNAL( newPendingMessage( Target* ) ), this, SLOT( showTrayMessage( Target* ) ) );
}

void PalringoWindow::showTrayMessage( Target* target )
{
    if( target->getPendingMessages().size() > 0 )
    {
        QString text;
        QString sender;

        Message message = target->getPendingMessages().last();
        if( message.type() == "text/plain" )
            text = message.payload();
        else if( message.type().startsWith( "image" ) )
            text = "Image message";
        else if ( message.type().startsWith( "audio" ) )
            text = "Audio message";
        else
            text = "Unknown message";

        if( message.bridgeID() > 0 )
        {
            sender = "Private message from " + Tools::getBridgeContact( message.bridgeID(), message.senderID() )->getNickname();
        }
        else
        {
            Contact* contact = qobject_cast<Contact*>( message.sender() );
            if( contact )
            {
                if( message.groupID() == 0 )
                {
                    sender = "Private message from - " + contact->getNickname();
                }
                else
                {
                    sender = "New message in " + Tools::getGroup( message.groupID() )->getName();
                    text = contact->getNickname() + ": " + text;
                }
            }
            else
            {
                return;
            }
        }

        this->systrayicon->showMessage( sender, text );
    }
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

void PalringoWindow::showSettingsWindow()
{
    if( this->settingsWindow == NULL )
    {
        this->settingsWindow = new SettingsWindow( this );
    }
    this->settingsWindow->show();
}

void PalringoWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("PalringoWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void PalringoWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("PalringoWindow");
    resize( settings.value( "size", QSize( 270, 500  )).toSize() );
    move( settings.value( "pos", QPoint( 80, 100 ) ).toPoint() );
    settings.endGroup();
}

void PalringoWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void PalringoWindow::cleanUp()
{
    for( int i = mainTabs->count(); i > 1; i-- )
    {
        QWidget *w = mainTabs->widget( i );
        mainTabs->removeTab( i );
        delete w;
    }
}
