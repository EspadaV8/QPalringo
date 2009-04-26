SOURCES += main.cpp \
    palringowindow.cpp \
    tools.cpp \
    settingswindow.cpp \
    speexdecoder.cpp \
    defaultChatWindow/chatwindow.cpp \
    defaultChatWindow/chattextedit.cpp \
    defaultChatWindow/messagelist.cpp \
    defaultChatWindow/messageitem.cpp \
    services/service.cpp \
    services/palringoservice.cpp \
    signinwindow.cpp \
    defaultListItems/contactlistitem.cpp \
    defaultListItems/grouplistitem.cpp \
    defaultListItems/listitem.cpp \
    defaultListItems/serviceitem.cpp
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
    debug
TARGET = ../bin/qpalringo
FORMS = ui/settingswindow.ui \
    ui/signinwindow.ui
QT += gui \
    svg \
    network
HEADERS += palringowindow.h \
    tools.h \
    settingswindow.h \
    speexdecoder.h \
    qpuiplugin.h \
    defaultChatWindow/chatwindow.h \
    defaultChatWindow/chattextedit.h \
    defaultChatWindow/messagelist.h \
    defaultChatWindow/messageitem.h \
    services/service.h \
    services/palringoservice.h \
    signinwindow.h \
    defaultListItems/contactlistitem.h \
    defaultListItems/grouplistitem.h \
    defaultListItems/listitem.h \
    defaultListItems/serviceitem.h
LIBS += -L../bin \
    -lQPalringo \
    -lssl \
    -lspeex \
    -lz \
    -lqp_default_ui
RESOURCES += ../resources/qpalringo.qrc
DISTFILES += ../qss/qpalringo.css \
    ../qss/listitem.qss \
    ../qss/messageitem.css
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
UI_DIR = ../tmp

INCLUDEPATH += ../libQPalringo
