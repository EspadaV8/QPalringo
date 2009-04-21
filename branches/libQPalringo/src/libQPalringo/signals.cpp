#include "qpalringoconnection.h"
#include "qpdatamap.h"
#include "qpgenericdata.h"

/*** out signals ***/
int QPalringoConnection::onPingSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Ping sent" );
    return 1;
}

int QPalringoConnection::onLogonSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Logon sent" );
    return 1;
}

int QPalringoConnection::onByeSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Bye sent" );
    RK_.clear();
    return 1;
}

int QPalringoConnection::onAuthSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Auth sent" );
    return 1;
}

int QPalringoConnection::onContactUpdateSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Contact Update sent" );
    return 1;
}

int QPalringoConnection::onContactAddRespSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Contact Add sent" );
    return 1;
}

int QPalringoConnection::onGroupSubscribeSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Group Subscribe sent" );
    return 1;
}

int QPalringoConnection::onGroupUnsubSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Group Unsub sent" );
    return 1;
}

int QPalringoConnection::onGroupCreateSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Group Create sent" );
    return 1;
}

int QPalringoConnection::onGroupInviteSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Group Invite sent" );
    return 1;
}

int QPalringoConnection::onGroupAdminSent( Headers&, QByteArray&, qpGenericData*  )
{
    qDebug( "Group Admin sent" );
    return 1;
}

int QPalringoConnection::onMesgSent( Headers& headers, QByteArray&, qpGenericData* )
{
    qpMsgData msgData;
    msgData.getData( headers );

    if( msgData.last_ == true )
    {
        qDebug( "Message sent" );
    }
    return 1;
}

int QPalringoConnection::onMesgStoredSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Message Stored sent" );
    return 1;
}

int QPalringoConnection::onMesgHistSent( Headers&, QByteArray&, qpGenericData* )
{
    qDebug( "Message History sent" );
    return 1;
}

int QPalringoConnection::onRegSent( Headers&, QByteArray&, qpGenericData*  )
{
    qDebug( "Reg sent" );
    return 1;
}
