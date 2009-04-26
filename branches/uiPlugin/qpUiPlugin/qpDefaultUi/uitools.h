#ifndef UITOOLS_H
#define UITOOLS_H

#include <QHash>

#include "targets/target.h"

#include "defaultChatWindow/chatwindow.h"
#include "contactpropertieswindow.h"

class uiTools;
extern uiTools *uitools_;

class uiTools : public QObject
{
    Q_OBJECT
    public:
        uiTools();
        bool checkChatWindowOpen( Target *target );
        void showContactProperties( Contact *contact );

    public slots:
        void openChatWindow( Target *target );

    private slots:
        void removeChatWindow( Target *target );

    private:
        QHash<Target*, ChatWindow*> openWindows;
};

#endif // UITOOLS_H
