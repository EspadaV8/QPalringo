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
#ifndef PALRINGOWINDOW_H
#define PALRINGOWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QScrollArea>
#include "targets/contact.h"
#include "targets/group.h"
#include "targets/target.h"
#include "signinwindow.h"
#include "messageitem.h"
#include "listitems/listitem.h"
#include "settingswindow.h"

class Connection;

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class PalringoWindow : public QMainWindow
{
    Q_OBJECT
    public:
        PalringoWindow();
        ~PalringoWindow();

        void sendMessage( Contact *contact, Message *message );

    public slots:
        void newGroupAdded( Group *group );
        void groupLeft( quint64 groupID );
        
    private slots:
        void joinAGroup();
        void createAGroup();
        void tabFocusChanged( int tabIndex );
        void showSettingsWindow();
        void cleanUp();
        void showTrayMessage( Target* target );
        void showOverview();
        void showContacts();
        void showGroup( quint64 groupID );

    private:
        QMenu *fileMenu;
            QMenu *palringoSubmenu;
                QAction *onlineMenuAction;
                QAction *awayMenuAction;
                QAction *busyMenuAction;
                QAction *invisibleMenuAction;
                QAction *offlineMenuAction;
                QAction *signinMenuAction;
            QAction *addServiceMenuAction;
            QAction *settingsMenuAction;
            QAction *exitMenuAction;

        QMenu *groupMenu;
            QAction *joinGroup;
            QAction *createGroup;

        QIcon *systraySvg;
        QSystemTrayIcon *systrayicon;

        void SetupActions();
        void CreateMenuBar();
        void UpdateContact();

        QScrollArea *qsa1;
        QTabWidget *mainTabs;
        QWidget *overviewPage;
        QWidget *contactsPage;

        QStackedWidget *pages;

        void setupUi();
        void loadUi();
        void initUiPlugin( QObject* plugin );
        void CreateTrayIcon();
        void SetupTabs();
        void setupButtonLayout();

        QList<ListItem *> usersContacts;
        QList<ListItem *> userServices;

        SettingsWindow* settingsWindow;

        void readSettings();
        void writeSettings();
        void closeEvent(QCloseEvent *event);
};

#endif
