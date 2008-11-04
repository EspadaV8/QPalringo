/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with QPalringo. If not, see <http://www.gnu.org/licenses/>       *
 *                                                                         *
 ***************************************************************************/
#ifndef CONTACTPROPERTIESWINDOW_H
#define CONTACTPROPERTIESWINDOW_H

#include <QDialog>
#include "targets/contact.h"
#include "ui_contactProperties.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class ContactPropertiesWindow : public QDialog, public Ui::ContactProperties
{
    Q_OBJECT
    public:
        static ContactPropertiesWindow* getInstance();
        void update( Contact *contact );
        
    protected:
        ContactPropertiesWindow();
        ContactPropertiesWindow( const ContactPropertiesWindow& );
        ContactPropertiesWindow& operator= ( const ContactPropertiesWindow& );

        ~ContactPropertiesWindow();
        
    private:
        static ContactPropertiesWindow* pinstance;
};

#endif
