#ifndef ONLINESTATUS_H
#define ONLINESTATUS_H
namespace qpOnlineStatus
{
    enum OnlineStatus
    {
        OFFLINE,
        ONLINE,
        AWAY,
        INVISIBLE,
        DND,
        BUSY,
        BRB,
        LUNCH,
        ON_PHONE,
        IDLE
    };

    const QString OnlineStatusText[] =
    {
        "Offline",
        "Online",
        "Away",
        "Invisible",
        "Do not disturb",
        "Busy",
        "BRB",
        "Out to lunch",
        "On the phone",
        "Idle"
    };
}
#endif // ONLINESTATUS_H
