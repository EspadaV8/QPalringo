#ifndef QPGROUPCAPABILITIES_H
#define QPGROUPCAPABILITIES_H
namespace qpGroupCapabilities
{
    enum GroupCapabilities
    {
        REGULAR     = 0x00,
        ADMIN       = 0x01,
        MOD         = 0x02,
        BANNED      = 0x04,
        SILENCED    = 0x08,
        HIDDEN      = 0x10
    };
}
#endif // QPGROUPCAPABILITIES_H
