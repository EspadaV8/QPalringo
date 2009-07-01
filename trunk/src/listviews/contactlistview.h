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
#ifndef CONTACTLISTVIEW_H
#define CONTACTLISTVIEW_H

#include "palringolistview.h"
#include "../libQPalringo/targets/contact.h"
#include "../libQPalringo/targets/bridgecontact.h"
#include "../listitems/contactlistitem.h"
#include "../listitems/bridgecontactlistitem.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class ContactListView : public PalringoListView
{
    Q_OBJECT
    public:
        ContactListView( QWidget *parent = 0 );
        ~ContactListView();

        virtual void setupContainers();

    protected:
        QHash<quint64, Contact*> contacts;

    protected slots:
        void contactReceived( Contact *contact, qint32 capabilities = 0 );
        void bridgeContactReceived( BridgeContact *contact );
        void getContacts( quint64 groupID = 0 );
        void checkContainerGroups( ListItem* );
        void removeContacts();

    public slots:
        virtual void inFocus();

};

#endif
