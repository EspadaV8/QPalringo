#include "qpdatamap.h"
#include <QtEndian>
#include <QMapIterator>

qpDataMap::qpDataMap()
{
}

qpDataMap::qpDataMap( const QByteArray& data )
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
        return;
    }

    QByteArray attr;
    QByteArray length;
    QByteArray value;

    int size = data.size();
    int i = 0;

    while( i < size )
    {
        int nullTerm  = data.indexOf( '\0', i );
        int lengthStart = nullTerm + 1;
        int dataStart = lengthStart + 2;
        attr = data.mid( i, nullTerm );

        if( attr.isEmpty() )
        {
            return;
        }

        length = data.mid( lengthStart, 2 );
        // extract the length of the data
        qint32 valueLength = qFromBigEndian<quint32>( (uchar*)length.data() );

        // out of bounds, parse error!
        if( ( dataStart + valueLength ) > size )
        {
            //throw DataMapException(OUT_OF_BOUNDS);
            return;
        }

        value = data.mid( dataStart, valueLength );

        if( dataMap_.contains( attr ) )
        {
            QByteArray a = dataMap_.value( attr );
            a.append( value );
            dataMap_.insert( attr, a );
        }
        else
        {
            dataMap_.insert( attr, value );
        }

        i = dataStart + valueLength;
    }
}

QString qpDataMap::toString()
{
    QString result = "{\n";

    QMapIterator<QByteArray, QByteArray> i(dataMap_);
    while( i.hasNext() )
    {
        result.append( "\t" );
        result.append( QString::fromUtf8( i.key().data() ) );
        result.append( ": " );
        result.append( QString::fromUtf8( i.value().data() ) );
        result.append( "\n" );
    }
    result.append("}");
    return result;
}
