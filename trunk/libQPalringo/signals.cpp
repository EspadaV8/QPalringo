#include "qpalringoconnection.h"
#include "qpdatamap.h"
#include "qpgenericdata.h"

/*** out signals ***/
int QPalringoConnection::onPingSent( Headers, QByteArray )
{
    qDebug( "Ping sent" );
    return 1;
}

int QPalringoConnection::onLogonSent( Headers, QByteArray )
{
    qDebug( "Logon sent" );
    return 1;
}

int QPalringoConnection::onByeSent( Headers, QByteArray )
{
    qDebug( "Bye sent" );
    RK_.clear();
    return 1;
}

int QPalringoConnection::onAuthSent( Headers, QByteArray )
{
    qDebug( "Auth sent" );
    return 1;
}

int QPalringoConnection::onContactUpdateSent( Headers, QByteArray )
{
    qDebug( "Contact Update sent" );
    return 1;
}

int QPalringoConnection::onContactAddRespSent( Headers, QByteArray )
{
    qDebug( "Contact Add sent" );
    return 1;
}

int QPalringoConnection::onGroupSubscribeSent( Headers, QByteArray )
{
    qDebug( "Group Subscribe sent" );
    return 1;
}

int QPalringoConnection::onGroupUnsubSent( Headers, QByteArray )
{
    qDebug( "Group Unsub sent" );
    return 1;
}

int QPalringoConnection::onGroupCreateSent( Headers, QByteArray )
{
    qDebug( "Group Create sent" );
    return 1;
}

int QPalringoConnection::onGroupInviteSent( Headers, QByteArray )
{
    qDebug( "Group Invite sent" );
    return 1;
}

int QPalringoConnection::onGroupAdminSent( Headers, QByteArray  )
{
    qDebug( "Group Admin sent" );
    return 1;
}

int QPalringoConnection::onMesgSent( Headers headers, QByteArray )
{
    qpMsgData msgData;
    msgData.getData( headers );

    if( msgData.last_ == true )
    {
        qDebug( "Message sent" );
    }
    return 1;
}

int QPalringoConnection::onMesgStoredSent( Headers, QByteArray )
{
    qDebug( "Message Stored sent" );
    return 1;
}

int QPalringoConnection::onMesgHistSent( Headers, QByteArray )
{
    qDebug( "Message History sent" );
    return 1;
}

int QPalringoConnection::onRegSent( Headers, QByteArray  )
{
    qDebug( "Reg sent" );
    return 1;
}
