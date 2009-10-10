#ifndef QPALPANE_H
#define QPALPANE_H

#include <QtGui>

class QPalPane : public QWidget
{
    Q_OBJECT
    public:
        QPalPane( QWidget* parent = 0 );
        virtual QString getTitle() const = 0;
};

#endif // QPALPANE_H
