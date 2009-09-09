#include <QtGui>
#include "qpdefaultui.h"

QWidget* QPDefaultUi::getCentralWidget()
{
    return new QPushButton( "Test" );
}

QString QPDefaultUi::getName()
{
    return "Default plugin";
}

Q_EXPORT_PLUGIN2(qpdefaultui, QPDefaultUi)
