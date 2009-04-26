#ifndef UITOOLS_H
#define UITOOLS_H

#include <QHash>

#include "targets/target.h"

#include "messageitem.h"
#include "chatwindow.h"
#include "contactpropertieswindow.h"

class uiTools;
extern uiTools *uitools_;

class uiTools : public QObject
{
    Q_OBJECT
    public:
        uiTools();
        bool checkChatWindowOpen( Target *target );
        void openChatWindow( Target *target );
        void removeChatWindow( Target *target );
        void showContactProperties( Contact *contact );

    private:
        QHash<Target*, ChatWindow *> openWindows;
};

#endif // UITOOLS_H
