#ifndef PRIVATECHATPANE_H
#define PRIVATECHATPANE_H

#include "targetpane.h"
#include "targets/contact.h"

class PrivateChatPane : public TargetPane
{
    Q_OBJECT
    public:
        PrivateChatPane( Contact* contact = 0, QWidget* parent = 0 );

    private:
        Contact* contact;
        QVBoxLayout* layout;
};

#endif // PRIVATECHATPANE_H
