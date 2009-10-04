#ifndef ERRORCODES_H
#define ERRORCODES_H
namespace qpErrorCodes
{
    enum ErrorCode
    {
        OK, /**< The command was executed with no error*/
        INTERNAL_ERROR,
        NO_SUCH_WHATEVER,
        NOT_A_MEMBER,
        DELIVERED, /**< Message delivered*/
        NOT_DELIVERED, /**< Message lost*/
        SFE_NOT_AVAILABLE,
        STATS_IF_NOT_AVAILABLE,
        END_OF_MESG_STORED,
        UNABLE_TO_STORE_OFFLINE_MESG,
        RESEND_CURRENT_MESG,
        GROUP_ALREADY_EXISTS,
        CONTACT_ALREADY_EXISTS,
        NOT_ALLOWED,
        NOT_AVAILABLE,
        THROTTLE,
        SUB_ALREADY_EXISTS,
        GROUP_FULL,
        BANNED,
        PAY_GROUP
    };
}
#endif // ERRORCODES_H
