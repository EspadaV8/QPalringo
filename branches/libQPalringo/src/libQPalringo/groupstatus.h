#ifndef GROUPSTATUS_H
#define GROUPSTATUS_H
namespace qpGroupStatus
{
    enum GroupStatus
    {
        GROUP_MEMBER_REGULAR       = 0x00,
        GROUP_MEMBER_ADMIN         = 0x01,
        GROUP_MEMBER_MOD           = 0x02,
        GROUP_MEMBER_BANNED        = 0x04,
        GROUP_MEMBER_SILENCED      = 0x08,
        GROUP_MEMBER_HIDDEN        = 0x10
    };
}
#endif // GROUPSTATUS_H
