#ifndef OVERVIEWPANE_H
#define OVERVIEWPANE_H

#include <QtGui>
#include "qpalpane.h"
#include "chatview.h"

class OverviewPane : public QPalPane
{
    Q_OBJECT
    public:
        OverviewPane( QWidget* parent = 0 );
        virtual QString getTitle() const;

    public slots:
        void addText( QString text );

    private:
        QHBoxLayout* layout;
        ChatView* statusView;
};

#endif // OVERVIEWPANE_H
