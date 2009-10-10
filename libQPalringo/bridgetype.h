#ifndef BRIDGETYPE_H
#define BRIDGETYPE_H
#include <QString>

namespace qpBridgeType
{
    enum Type
    {
        PALRINGO,   // 0
        MSN = 1,    // 1
        YAHOO,      // 2
        AIM,        // 3
        GADUGADU,   // 4
        ICQ,        // 5
        XMPP,       // 6
        QQ,         // 7
        FACEBOOK    // 8
    };

    const QString TypeText[] =
    {
        "Palringo",
        "MSN",
        "Yahoo!",
        "AIM",
        "Gadu-Gadu",
        "ICQ",
        "XMPP",
        "QQ",
        "Facebook"
    };
}

#endif // BRIDGETYPE_H
