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

class qpGenericData
{
    public:
        qpGenericData();

        virtual Headers getData( QString body = "" ) = 0;
        virtual Headers setData( QString body = "" ) = 0;

        quint64 mesgId_;
};

class qpMsgData : public qpGenericData
{
    public:
        qpMsgData();

        Headers getData( QString body = "" );
        Headers setData( QString body = "" );

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

        Headers getData( QString body = "" );
        Headers setData( QString body = "" );

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

        Headers getData( QString body = "" );
        Headers setData( QString body = "" );

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

        Headers getData( QString body = "" );
        Headers setData( QString body = "" );

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

        Headers getData( QString body = "" );
        Headers setData( QString body = "" );

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

        Headers getData( QString body = "" );
        Headers setData( QString body = "" );

        qpWhat::What what_;
        qpErrorCodes::ErrorCode errorCode_;
        QString errorMessage_;
        qint32 type_;
        quint32 contentLength_;
};


#endif // QPGENERICDATA_H