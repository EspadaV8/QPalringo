//
// C++ Interface: contactpropertieswindow
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
