#include "qpdatamap.h"
#include <QtEndian>
#include <QMapIterator>
#include <QDebug>

qpDataMap::qpDataMap()
    : QMap<QString, QByteArray>()
{
}

qpDataMap::qpDataMap( const QByteArray& data )
    : QMap<QString, QByteArray>()
{
    parse( data );
}

qpDataMap::~qpDataMap()
{
}

void qpDataMap::parse( QByteArray data )
{
    if( data.isNull() )
    {
        qDebug( "data is null" );
        return;
    }

    QString attr;
    QByteArray length;
    QByteArray value;

    int size = data.size();
    int i = 0;

    while( i < size )
    {
        int nullTerm  = data.indexOf( '\0', i );
        int lengthStart = nullTerm + 1;
        int dataStart = lengthStart + 2;
        attr = (data.mid( i, nullTerm - i )).toUpper();

        if( attr.isEmpty() )
        {
            return;
        }

        length = data.mid( lengthStart, 2 );
        // extract the length of the data
        qint32 valueLength = 0;
        for( int j = 0; j < length.size(); j++ )
        {
            valueLength <<= 8;
            char c = length.at( j );
            valueLength |= ( c & 0xff ) ;
        }

        // out of bounds, parse error!
        if( ( dataStart + valueLength ) > size )
        {
            //throw DataMapException(OUT_OF_BOUNDS);
            return;
        }

        value = data.mid( dataStart, valueLength );

        if( this->contains( attr ) )
        {
            QByteArray a = this->value( attr );
            a.append( value );
            this->insert( attr, a );
        }
        else
        {
            this->insert( attr, value );
        }

        i = dataStart + valueLength;
    }
}

QString qpDataMap::toString()
{
    QString result = "{\n";

    QMapIterator<QString, QByteArray> i( *this );
    while( i.hasNext() )
    {
        i.next();
        result.append( "\t" );
        result.append( i.key() );
        result.append( ": " );
        result.append( i.value() );
        result.append( "\n" );
    }
    result.append("}");
    return result;
}
