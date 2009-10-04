#ifndef QPDATAMAP_H
#define QPDATAMAP_H

#include <QString>
#include <QByteArray>
#include <QMap>

class qpDataMap : public QMap<QString, QByteArray>
{
private:
    void parse( QByteArray data );

public:
    static const char *EMPTY;
    static const char *OUT_OF_BOUNDS;

    qpDataMap( const QByteArray& data );
    qpDataMap();
    ~qpDataMap();

    QByteArray getData();
    QString toString();
};
#endif // QPDATAMAP_H
