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
        qDebug( "data is null" );
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
            qDebug( "attr is empty" );
            return;
        }

        length = data.mid( lengthStart, 2 );
        // extract the length of the data
        qint32 valueLength = qFromBigEndian<quint32>( (uchar*)length.data() );

        // out of bounds, parse error!
        if( ( dataStart + valueLength ) > size )
        {
            qDebug( "out of bounds, parse error - %d - %d - %d", dataStart, valueLength, size );
            //throw DataMapException(OUT_OF_BOUNDS);
            return;
        }

        value = data.mid( dataStart, valueLength );

        if( dataMap_.contains( attr ) )
        {
            qDebug( "dataMap already contains attr" );
            QByteArray a = dataMap_.value( attr );
            a.append( value );
            dataMap_.insert( attr, a );
        }
        else
        {
            qDebug( "dataMap doesn't contain attr" );
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
