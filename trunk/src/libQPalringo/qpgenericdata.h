#ifndef QPGENERICDATA_H
#define QPGENERICDATA_H

#include <QString>
#include "headers.h"
#include "qpdatamap.h"
#include "onlinestatus.h"
#include "devicetype.h"
#include "groupstatus.h"
#include "what.h"
#include "errorcodes.h"
#include "bridgetype.h"

class qpGenericData
{
    public:
        qpGenericData();

        virtual void getData( const Headers& headers, QByteArray body = "" ) = 0;
        virtual Headers setData( QByteArray body = "" );

        quint64 mesgId_;
};

class qpMsgData : public qpGenericData
{
    public:
        qpMsgData();

        void getData( const Headers& headers, QByteArray body = "" );
        Headers setData( QByteArray body = "" );

        quint64 sourceId_;
        quint64 targetId_;
        quint32 mesgTarget_;
        quint32 correlationId_;
        quint32 contentLength_;
        quint32 totalLength_;

        QString contentType_;
        QString email_;
        QString name_;
        QString timestamp_;

        bool last_;
        bool hist_;
};

class qpLogonData : public qpGenericData
{
    public:
        qpLogonData();

        void getData( const Headers& headers, QByteArray body = "" );

        quint64 subId_;
        QString name_;
        QString nickname_;
        QString status_;
        QString lastOnline_;
        QString reason_;
        QString timestamp_;
        qpDataMap *dataMap_;
};

class qpAuthData : public qpGenericData
{
    public:
        qpAuthData();

        void getData( const Headers& headers, QByteArray body = "" );
        Headers setData( QByteArray body = "" );

        qint32 encryptionType_; // -1 means unset
        qint32 ghost_; // -1 means unset
        quint32 wordSize_;
        quint32 contentLength_;

        QString name_;
        qpOnlineStatus::OnlineStatus onlineStatus_; // -1 means unset
};

class qpContactData : public qpGenericData
{
    public:
        qpContactData();

        void getData( const Headers& headers, QByteArray body = "" );

        quint64 contactId_;
        quint64 capabilities_;
        quint64 sourceId_;
        quint64 targetId_;
        quint32 contentLength_;

        QString nickname_;
        QString status_;
        QString name_;

        qpDeviceType::DeviceType deviceType_;
        qpOnlineStatus::OnlineStatus onlineStatus_; // -1 means unset

        bool isContact_;
        bool remove_;
        bool block_;
};

class qpGroupData : public qpGenericData
{
    public:
        qpGroupData();

        void getData( const Headers& headers, QByteArray body = "" );

        quint64 groupId_;
        quint64 contactId_;
        quint64 targetId_;
        quint32 contentLength_;
        qint32 type_; // -1 means unset

        QString name_;
        QString message_;
        QString admin_;
        QString desc_;

        qpGroupStatus::GroupStatus action_; // -1 means unset
};

class qpResponseData : public qpGenericData
{
    public:
        qpResponseData();

        void getData( const Headers& headers, QByteArray body = "" );

        qpWhat::What what_;
        qpErrorCodes::ErrorCode errorCode_;
        QString errorMessage_;
        qint32 type_;
        quint32 contentLength_;
};

class qpBridgeData : public qpGenericData
{
    public:
        qpBridgeData();

        void getData( const Headers& headers, QByteArray body = "" );

        quint32 bridgeId_;
        qpBridgeType::Type type_;
        QString nickname_;
        QString username_;
};

class qpBridgeContactData : public qpGenericData
{
    public:
        qpBridgeContactData();

        void getData( const Headers& headers, QByteArray body = "" );

        quint32 bridgeId_;
        quint64 contactId_;
        qpOnlineStatus::OnlineStatus onlineStatus_; // -1 means unset
        QString name_;
        QString nickname_;
        QString status_;
        QString currentMedia_;
};

#endif // QPGENERICDATA_H
