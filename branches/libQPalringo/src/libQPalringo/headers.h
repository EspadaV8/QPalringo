#ifndef HEADERS_H
#define HEADERS_H

#include <QHash>
#include <QVariant>

class Headers : public QHash<QString, QVariant>
{
    public:
        Headers();

        template <class T> T attribute( const QString& key )
        {
            QVariant v = value( key );
            return v.value<T>();
        }
};

#endif // HEADERS_H
