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

Headers qpMsgData::getData( Headers headers, QByteArray )
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

Headers qpLogonData::getData( Headers headers, QByteArray body )
{
    dataMap_ = new qpDataMap( body );

    qDebug( "%s", qPrintable( dataMap_->toString() ) );

    subId_      = headers.attribute<quint64>( qpHeaderAttribute::SUB_ID );
    name_       = headers.attribute<QString>( qpHeaderAttribute::NAME );
    nickname_   = headers.attribute<QString>( qpHeaderAttribute::EMAIL );
    status_     = headers.attribute<QString>( qpHeaderAttribute::NICKNAME );
    lastOnline_ = headers.attribute<QString>( qpHeaderAttribute::LAST_ONLINE );
    reason_     = headers.attribute<QString>( qpHeaderAttribute::REASON );
    timestamp_  = headers.attribute<QString>( qpHeaderAttribute::TIMESTAMP );

    return headers;
}

Headers qpLogonData::setData( QByteArray )
{
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

Headers qpAuthData::getData( Headers, QByteArray )
{
    /*
    getAttribute<uint32_t, true>(headers, "WORD-SIZE", wordSize_);
    getAttribute<int32_t, true>(headers, "ENCRYPTION-TYPE", encryptionType_);
    */
}

Headers qpAuthData::setData( QByteArray )
{
    /*
    if (encryptionType_ > -1)
    {
        headers["ENCRYPTION-TYPE"] = toString(encryptionType_);
    }

    if (name_.size())
    {
        headers["NAME"] = name_;
    }

    if (onlineStatus_ > -1)
    {
        headers["ONLINE-STATUS"] = toString(onlineStatus_);
    }
    */
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

Headers qpContactData::getData( Headers, QByteArray )
{
    /*
    getAttribute<uint32_t, true>(headers, "MESG-ID", mesgId_);
    getAttribute<uint64_t, true>(headers, "CONTACT-ID", contactId_);
    getAttribute<OnlineStatus, true>(headers, "ONLINE-STATUS", onlineStatus_);
    getAttribute<std::string, true>(headers, "NICKNAME", nickname_);
    getAttribute<std::string, true>(headers, "STATUS", status_);
    getAttribute<DeviceType, true>(headers, "DEVICE-TYPE", deviceType_);
    getAttribute<bool>(headers, "REMOVE", remove_);
    getAttribute<bool>(headers, "BLOCK", block_);
    getAttribute<uint64_t, true>(headers, "CAPABILITIES", capabilities_);
    getAttribute<uint64_t, true>(headers, "SOURCE-ID", sourceId_);
    getAttribute<uint64_t, true>(headers, "TARGET-ID", targetId_);
    getAttribute<std::string, true>(headers, "NAME", name_);
    getAttribute<uint32_t, true>(headers, "CONTENT-LENGTH", contentLength_);
    getAttribute<bool>(headers, "CONTACT", isContact_);
    */
}

Headers qpContactData::setData( QByteArray )
{
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

Headers qpGroupData::getData( Headers, QByteArray )
{
    /*
    getAttribute<uint32_t, true>(headers, "MESG-ID", mesgId_);
    getAttribute<uint64_t, true>(headers, "GROUP-ID", groupId_);
    getAttribute<std::string, true>(headers, "NAME", name_);
    getAttribute<uint32_t, true>(headers, "CONTENT-LENGTH", contentLength_);
    getAttribute<std::string, true>(headers, "ADMIN", admin_);
    getAttribute<uint64_t, true>(headers, "CONTACT-ID", contactId_);
    getAttribute<int32_t, true>(headers, "TYPE", type_);
    getAttribute<std::string, true>(headers, "DESC", desc_);
    getAttribute<std::string, true>(headers, "MESSAGE", message_);
    getAttribute<uint64_t, true>(headers, "TARGET-ID", targetId_);
    getAttribute<GroupStatus, true>(headers, "ACTION", action_);
    */
}

Headers qpGroupData::setData( QByteArray )
{
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

Headers qpResponseData::getData( Headers, QByteArray )
{
    /*
    getAttribute<uint32_t>(headers, "MESG-ID", mesgId_);
    getAttribute<What>(headers, "WHAT", what_);
    getAttribute<int32_t>(headers, "TYPE", type_);
    getAttribute<uint32_t>(headers, "CONTENT_LENGTH", contentLength_);

    if (type_)
    {
        errorMessage_ = body;
    }
    else
    {
        errorCode_ = static_cast<ErrorCode>(ntohl(
                *reinterpret_cast<const int*>(body.data() + 4)));
    }
    */
}

Headers qpResponseData::setData( QByteArray )
{
}
/**
 * END INLINE CLASSES
 */
