#include "qpgenericdata.h"
#include "qpheaderattributes.h"

/**
 * INLINE CLASSES
 */
// TODO: put these in a namespace
/**
 * qpGenericData
 */
qpGenericData::qpGenericData()
{
    mesgId_ = 0;
}

Headers qpGenericData::setData( QByteArray )
{
    Headers h;
    return h;
}

/**
 * qpMsgData
 */
qpMsgData::qpMsgData()
    : qpGenericData()
{
    sourceId_ = 0;
    targetId_ = 0;
    mesgTarget_ = 0;
    correlationId_ = 0;
    contentLength_ = 0;
    totalLength_ = 0;
    last_ = false;
}

void qpMsgData::getData( const Headers& headers, QByteArray )
{
    mesgId_         = headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
    targetId_       = headers.attribute<quint64>( qpHeaderAttribute::TARGET_ID );
    sourceId_       = headers.attribute<quint64>( qpHeaderAttribute::SOURCE_ID );

    mesgTarget_     = headers.attribute<quint32>( qpHeaderAttribute::MESG_TARGET );
    correlationId_  = headers.attribute<quint32>( qpHeaderAttribute::CORRELATION_ID );
    contentLength_  = headers.attribute<quint32>( qpHeaderAttribute::CONTENT_LENGTH );
    totalLength_    = headers.attribute<quint32>( qpHeaderAttribute::TOTAL_LENGTH );

    contentType_    = headers.attribute<QString>( qpHeaderAttribute::CONTENT_TYPE );
    email_          = headers.attribute<QString>( qpHeaderAttribute::EMAIL );
    name_           = headers.attribute<QString>( qpHeaderAttribute::NAME );
    timestamp_      = headers.attribute<QString>( qpHeaderAttribute::TIMESTAMP );

    last_           = headers.attribute<bool>( qpHeaderAttribute::LAST );
    hist_           = headers.attribute<bool>( qpHeaderAttribute::HIST );
}

Headers qpMsgData::setData( QByteArray )
{
    Headers headers;

    headers.insert( qpHeaderAttribute::MESG_ID, mesgId_ );
    headers.insert( qpHeaderAttribute::MESG_TARGET, mesgTarget_ );

    if( targetId_ > 0 )
    {
        headers.insert( qpHeaderAttribute::TARGET_ID, targetId_ );
    }

    if( correlationId_ > 0 )
    {
        headers.insert( qpHeaderAttribute::CORRELATION_ID, correlationId_ );
    }

    if( contentType_.size() > 0 )
    {
        headers.insert( qpHeaderAttribute::CONTENT_TYPE, contentType_ );
    }

    if( contentLength_ > 0 )
    {
        headers.insert( qpHeaderAttribute::CONTENT_LENGTH, contentLength_ );
    }

    if( last_ == true )
    {
        headers.insert( qpHeaderAttribute::LAST, "T" );
    }

    return headers;
}

/**
 * qpLogonData
 */
qpLogonData::qpLogonData()
    : qpGenericData()
{
    subId_ = 0;
}

void qpLogonData::getData( const Headers& headers, QByteArray body )
{
    dataMap_ = new qpDataMap( body );

    subId_      = headers.attribute<quint64>( qpHeaderAttribute::SUB_ID );
    name_       = headers.attribute<QString>( qpHeaderAttribute::NAME );
    nickname_   = headers.attribute<QString>( qpHeaderAttribute::NICKNAME );
    status_     = headers.attribute<QString>( qpHeaderAttribute::STATUS );
    lastOnline_ = headers.attribute<QString>( qpHeaderAttribute::LAST_ONLINE );
    reason_     = headers.attribute<QString>( qpHeaderAttribute::REASON );
    timestamp_  = headers.attribute<QString>( qpHeaderAttribute::TIMESTAMP );
}

/**
 * qpAuthData
 */
qpAuthData::qpAuthData()
    : qpGenericData()
{
    encryptionType_ = -1;
    wordSize_ = 0;
    onlineStatus_ = static_cast<qpOnlineStatus::OnlineStatus>( -1 );
    ghost_ = -1;
    contentLength_ = 0;
}

void qpAuthData::getData( const Headers& headers, QByteArray )
{
    wordSize_       = headers.attribute<quint32>( qpHeaderAttribute::WORD_SIZE );
    encryptionType_ = headers.attribute<qint32>( qpHeaderAttribute::ENCRYPTION_TYPE );
}

Headers qpAuthData::setData( QByteArray )
{
    Headers headers;
    headers.insert( qpHeaderAttribute::ENCRYPTION_TYPE, encryptionType_ );
    headers.insert( qpHeaderAttribute::NAME, name_ );
    headers.insert( qpHeaderAttribute::ONLINE_STATUS, onlineStatus_ );

    return headers;
}

/**
 * qpContactData
 */
qpContactData::qpContactData()
    : qpGenericData()
{
    contactId_ = 0;
    onlineStatus_ = static_cast<qpOnlineStatus::OnlineStatus>( -1 );
    deviceType_ = static_cast<qpDeviceType::DeviceType>( 0 );
    remove_ = false;
    block_ = false;
    capabilities_ = 0;
    sourceId_ = 0;
    targetId_ = 0;
    contentLength_ = 0;
}

void qpContactData::getData( const Headers& headers, QByteArray )
{
    mesgId_ = headers.attribute<qint32>( qpHeaderAttribute::MESG_ID );
    contentLength_ = headers.attribute<qint32>( qpHeaderAttribute::CONTENT_LENGTH );
    onlineStatus_ = (qpOnlineStatus::OnlineStatus) headers.attribute<qint32>( qpHeaderAttribute::ONLINE_STATUS );
    nickname_ = headers.attribute<QString>( qpHeaderAttribute::NICKNAME );
    status_ = headers.attribute<QString>( qpHeaderAttribute::STATUS );
    name_ = headers.attribute<QString>( qpHeaderAttribute::NAME );
    remove_  = headers.attribute<bool>( qpHeaderAttribute::REMOVE );
    block_  = headers.attribute<bool>( qpHeaderAttribute::BLOCK );
    isContact_ = headers.attribute<bool>( qpHeaderAttribute::IS_CONTACT );
    contactId_ = headers.attribute<quint64>( qpHeaderAttribute::CONTACT_ID );
    capabilities_ = headers.attribute<quint64>( qpHeaderAttribute::CAPABILITIES );
    sourceId_ = headers.attribute<quint64>( qpHeaderAttribute::SOURCE_ID );
    targetId_ = headers.attribute<quint64>( qpHeaderAttribute::TARGET_ID );
    deviceType_ = (qpDeviceType::DeviceType)headers.attribute<qint32>( qpHeaderAttribute::DEVICE_TYPE );
}

/**
 * qpGroupData
 */
qpGroupData::qpGroupData()
    : qpGenericData()
{
    groupId_ = 0;
    contentLength_ = 0;
    contactId_ = 0;
    type_ = 1;
    targetId_ = 0;
    action_ = static_cast<qpGroupStatus::GroupStatus>( -1 );
}

void qpGroupData::getData( const Headers& headers, QByteArray )
{
    mesgId_ = headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
    groupId_ = headers.attribute<quint64>( qpHeaderAttribute::GROUP_ID );
    contactId_ = headers.attribute<quint64>( qpHeaderAttribute::CONTACT_ID );
    targetId_ = headers.attribute<quint64>( qpHeaderAttribute::TARGET_ID );
    contentLength_ = headers.attribute<quint32>( qpHeaderAttribute::CONTENT_LENGTH );
    type_ = headers.attribute<qint32>( qpHeaderAttribute::TYPE );
    message_ = headers.attribute<QString>( qpHeaderAttribute::MESSAGE );
    name_ = headers.attribute<QString>( qpHeaderAttribute::NAME );
    admin_ = headers.attribute<QString>( qpHeaderAttribute::ADMIN );
    desc_ = headers.attribute<QString>( qpHeaderAttribute::DESC );
    action_ = (qpGroupStatus::GroupStatus)headers.attribute<qint32>( qpHeaderAttribute::ACTION );
}

/**
 * qpResponseData
 */
qpResponseData::qpResponseData()
    : qpGenericData()
{
    what_ = static_cast<qpWhat::What>( -1 );
    errorCode_ = static_cast<qpErrorCodes::ErrorCode>( -1 );
    type_ = -1;
    contentLength_ = -1;
}

void qpResponseData::getData( const Headers& headers, QByteArray body )
{
    mesgId_ = headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
    contentLength_ = headers.attribute<quint64>( qpHeaderAttribute::CONTENT_LENGTH );
    type_ = headers.attribute<quint64>( qpHeaderAttribute::TYPE );
    what_ = (qpWhat::What)headers.attribute<quint32>( qpHeaderAttribute::WHAT );

    if( type_ )
    {
        errorMessage_ = body;
    }
    else
    {
        int temp = 0;
        for( int i = 0; i < body.size(); i++ )
        {
            temp <<= 8;
            char c = body.at( i );
            temp |= ( c & 0xff ) ;
        }
        errorCode_ = static_cast<qpErrorCodes::ErrorCode>( temp );
    }
}

/**
 * qpBridgeData
 */
qpBridgeData::qpBridgeData()
    : qpGenericData()
{
    bridgeId_ = 0;
    type_ = static_cast<qpBridgeType::Type>( -1 );
    nickname_ = "";
    username_ = "";
}

void qpBridgeData::getData( const Headers& headers, QByteArray )
{
    mesgId_ = headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
    bridgeId_ = headers.attribute<quint32>( qpHeaderAttribute::BRIDGE_ID );
    type_ = (qpBridgeType::Type)headers.attribute<quint64>( qpHeaderAttribute::BRIDGE_TYPE );
    nickname_ = headers.attribute<QString>( qpHeaderAttribute::NICKNAME );
    username_ = headers.attribute<QString>( qpHeaderAttribute::USERNAME );
}

/**
 * qpBridgeContactData
 */
qpBridgeContactData::qpBridgeContactData()
    : qpGenericData()
{
    bridgeId_ = 0;
    contactId_ = 0;
    name_ = "";
    nickname_ = "";
}

void qpBridgeContactData::getData( const Headers& headers, QByteArray )
{
    mesgId_ = headers.attribute<quint64>( qpHeaderAttribute::MESG_ID );
    bridgeId_ = headers.attribute<quint32>( qpHeaderAttribute::BRIDGE_ID );
    contactId_ = headers.attribute<quint64>( qpHeaderAttribute::CONTACT_ID );
    name_ = headers.attribute<QString>( qpHeaderAttribute::NAME );
    nickname_ = headers.attribute<QString>( qpHeaderAttribute::NICKNAME );
}
/**
 * END INLINE CLASSES
 */
