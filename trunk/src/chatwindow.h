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
#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QDateTime>
#include <QNetworkAccessManager>
#include "palringowindow.h"
#include "targets/target.h"
#include "messagelist.h"
#include "chattextedit.h"

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class ChatWindow : public QWidget
{
Q_OBJECT
    public:
        ChatWindow(PalringoWindow *parent = 0, Target *target = NULL );
        ~ChatWindow();

        void appendMessage( Message message );

    private slots:
        void checkMessageInput();
        void askForHistory();
        void loadImageFile();
        void getMessages();
        void handleNetworkData( QNetworkReply *networkReply );

    private:
        PalringoWindow *parent;
        Target *target;

        QPushButton *historyButton;
        QPushButton *voiceButton;
        QPushButton *imageButton;
        MessageList *messageList;
        ChatTextEdit *multiLineInput;
        QNetworkAccessManager networkAccessManager;
        
        void sendImageMessage( QImage image );
        void sendTextMessage( QString message );
        void sendMessage( Message message );

    protected:
        virtual void keyPressEvent( QKeyEvent *event );
        virtual void dragEnterEvent( QDragEnterEvent *event );
        virtual void dropEvent( QDropEvent *event );
};

#endif

