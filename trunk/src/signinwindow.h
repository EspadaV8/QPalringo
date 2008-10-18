//
// C++ Interface: signinwindow
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include "ui_signinwindow.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class SigninWindow : public QDialog, public Ui::QPalringoSigninWindow
{
Q_OBJECT
    public:
        SigninWindow(QWidget *parent = 0);
        ~SigninWindow();

        QString getEmailAddress();
        QString getPassword();

    private slots:
        void openPalringoConnection();
        void setFeildValues();

};

#endif
