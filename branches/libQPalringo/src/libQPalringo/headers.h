#ifndef HEADERS_H
#define HEADERS_H

#include <QHash>
#include <QVariant>

class Headers : public QHash<QString, QVariant>
{
    public:
        Headers();

        QString toString() const;

        template <class T> T attribute( const QString& key ) const
        {
            QVariant v = value( key );
            return v.value<T>();
        }
};

#endif // HEADERS_H
