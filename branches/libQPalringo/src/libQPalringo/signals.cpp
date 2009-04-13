#include "qpalringoconnection.h"
#include "qpdatamap.h"



/*** out signals ***/
int QPalringoConnection::onPingSent(Headers headers __attribute__ ((unused)),
                                    QByteArray body __attribute__ ((unused)),
                                    qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Ping sent" );
    return 1;
}

int QPalringoConnection::onLogonSent(Headers headers __attribute__ ((unused)),
                                     QByteArray body __attribute__ ((unused)),
                                     qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Logon sent" );
    return 1;
}

int QPalringoConnection::onByeSent(Headers headers __attribute__ ((unused)),
                                   QByteArray body __attribute__ ((unused)),
                                   qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Bye sent" );
    RK_.clear();
    poll();
    return 1;
}

int QPalringoConnection::onAuthSent(Headers headers __attribute__ ((unused)),
                                    QByteArray body __attribute__ ((unused)),
                                    qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Auth sent" );
    return 1;
}

int QPalringoConnection::onContactUpdateSent(Headers headers __attribute__ ((unused)),
                                             QByteArray body __attribute__ ((unused)),
                                             qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Contact Update sent" );
    return 1;
}

int QPalringoConnection::onContactAddRespSent(Headers headers __attribute__ ((unused)),
                                              QByteArray body __attribute__ ((unused)),
                                              qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Contact Add sent" );
    return 1;
}

int QPalringoConnection::onGroupSubscribeSent(Headers headers __attribute__ ((unused)),
                                              QByteArray body __attribute__ ((unused)),
                                              qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Group Subscribe sent" );
    return 1;
}

int QPalringoConnection::onGroupUnsubSent(Headers headers __attribute__ ((unused)),
                                          QByteArray body __attribute__ ((unused)),
                                          qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Group Unsub sent" );
    return 1;
}
int QPalringoConnection::onGroupCreateSent(Headers headers __attribute__ ((unused)),
                                           QByteArray body __attribute__ ((unused)),
                                           qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Group Create sent" );
    return 1;
}

int QPalringoConnection::onGroupInviteSent(Headers headers __attribute__ ((unused)),
                                           QByteArray body __attribute__ ((unused)),
                                           qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Group Invite sent" );
    return 1;
}

int QPalringoConnection::onGroupAdminSent(Headers headers __attribute__ ((unused)),
                                          QByteArray body __attribute__ ((unused)),
                                          qpGenericData *data __attribute__ ((unused)) )
{
    qDebug( "Group Admin sent" );
    return 1;
}

int QPalringoConnection::onMesgSent(Headers headers __attribute__ ((unused)),
                                    QByteArray body __attribute__ ((unused)),
                                    qpGenericData *data)
{
    qpMsgData *msgData = dynamic_cast<qpMsgData*>( data );

    if( msgData->last_ == true )
    {
        qDebug( "Message sent" );
    }
    return 1;
}

int QPalringoConnection::onMesgStoredSent(Headers headers __attribute__ ((unused)),
                                          QByteArray body __attribute__ ((unused)),
                                          qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Message Stored sent" );
    return 1;
}

int QPalringoConnection::onMesgHistSent(Headers headers __attribute__ ((unused)),
                                        QByteArray body __attribute__ ((unused)),
                                        qpGenericData *data __attribute__ ((unused)))
{
    qDebug( "Message History sent" );
    return 1;
}

int QPalringoConnection::onRegSent(Headers headers __attribute__ ((unused)),
                                   QByteArray body __attribute__ ((unused)),
                                   qpGenericData *data __attribute__ ((unused)) )
{
    qDebug( "Reg sent" );
    return 1;
}
