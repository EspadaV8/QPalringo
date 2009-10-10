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
        virtual QString getTitle() const;

    protected:
        Target* target;
        ChatView* chatView;

    signals:
        emit void updateTargetIcon( Target* target );
};

#endif // TARGETPANE_H
