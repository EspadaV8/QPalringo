#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QtGui>
#include "message.h"

class ChatView : public QWidget
{
    Q_OBJECT
    public:
        ChatView( QWidget* parent = 0, QString title = "Unknown", QString iconFilename = ":/svg/logo.svg" );
        void addText( QString text );
        void insertMarker();
        void setTitle( QString title );
        void setIcon( QString iconFilename );
        void appendMessage( Message message );

    signals:
        void sendMessage( QString text );

    private:
        QTextBrowser* chatview;
        QVBoxLayout* layout;

        QHBoxLayout* titleLayout;
        QLabel* title;
        QLabel* icon;

        QLineEdit* textInput;

        bool marker;

    private slots:
        void sendText();
};

#endif // CHATVIEW_H
