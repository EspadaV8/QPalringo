#include "headers.h"
#include <QVariant>

Headers::Headers()
        : QHash<QString, QVariant>()
{
}

QString Headers::toString() const
{
    QString s = "{\n";
    QHashIterator<QString, QVariant> i( *this );
    while( i.hasNext() )
    {
        i.next();
        s.append( "\t" );
        s.append( i.key() );
        s.append( ": " );
        s.append( i.value().toString() );
        s.append( "\n" );
    }
    s.append( "}\n" );

    return s;
}
