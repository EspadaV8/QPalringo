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
#ifndef PALRINGOWINDOW_H
#define PALRINGOWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "palringolistview.h"
#include "contact.h"
#include "group.h"
#include "signinwindow.h"
#include "messageitem.h"

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
        void showSigninWindow();
        void loginService();
        void newGroupAdded( Group *group );

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

        QPixmap *systraypixmap;
        QSystemTrayIcon *systrayicon;

        void SetupActions();
        void CreateMenuBar();
        void UpdateContact();

        QScrollArea *qsa1;
        QTabWidget *mainTabs;
        QWidget *overviewPage;
        QWidget *contactsPage;

        void CreateTrayIcon();
        void SetupTabs();
        void AddPalringoService();

        PalringoListView *overviewList;
        PalringoListView *contactList;
        PalringoListView *groupList;

        QList<ListItem *> usersContacts;
        QList<ListItem *> userServices;

        SigninWindow *signinWindow;
};

#endif
