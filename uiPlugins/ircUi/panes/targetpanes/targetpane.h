#ifndef TARGETPANE_H
#define TARGETPANE_H

#include "../qpalpane.h"
#include "targets/target.h"
#include "chatview.h"

class TargetPane : public QPalPane
{
    Q_OBJECT
    public:
        TargetPane( Target* target = 0, QWidget* parent = 0 );
        void addText( QString text );
        void appendMessage( Message message );
        virtual QString getTitle() const;

    protected:
        Target* target;
        ChatView* chatView;

        void insertMessages();
        void showEvent( QShowEvent* event );
        void hideEvent( QHideEvent* event );

    protected slots:
        void handleNewMessage( Target* target );
        void sendTextMessage( QString text );

    signals:
        void updateTargetIcon( Target* target );
};

#endif // TARGETPANE_H
