//
// C++ Interface: chatwindow
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSplitter>
#include "palringowindow.h"
#include "palringocontact.h"
#include "messagelist.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class ChatWindow : public QWidget
{
Q_OBJECT
    public:
        ChatWindow(PalringoWindow *parent = 0, Contact *contact = NULL );
        ~ChatWindow();

        void appendMessage( QString message, Contact *contact, QString contentType );

    private slots:
        void checkMessageInput();

    private:
        PalringoWindow *parent;
        Contact *contact;

        QVBoxLayout *layout;
        QHBoxLayout *hbox;

        QPushButton *historyButton;
        QPushButton *voiceButton;
        QPushButton *imageButton;
        MessageList *messageList;
        QLineEdit *messageInput;
};

#endif
