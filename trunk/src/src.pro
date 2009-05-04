SOURCES += main.cpp \
    palringowindow.cpp \
    listviews/palringolistview.cpp \
    listviews/listviewcontainer.cpp \
    tools.cpp \
    chatwindow.cpp \
    chattextedit.cpp \
    messagelist.cpp \
    messageitem.cpp \
    signinwindow.cpp \
    settingswindow.cpp \
    listitems/contactlistitem.cpp \
    listitems/listitem.cpp \
    listitems/serviceitem.cpp \
    listitems/grouplistitem.cpp \
    speexdecoder.cpp \
    services/service.cpp \
    services/palringoservice.cpp \
    contactpropertieswindow.cpp \
    listviews/contactlistview.cpp \
    listviews/grouplistview.cpp \
    listviews/overviewlistview.cpp \
    services/bridgeservice.cpp

TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
    debug
TARGET = ../bin/qpalringo
FORMS = ui/signinwindow.ui \
    ui/contactProperties.ui \
    ui/settingswindow.ui
QT += gui \
    svg \
    network
HEADERS += palringowindow.h \
    listviews/palringolistview.h \
    listviews/listviewcontainer.h \
    tools.h \
    chatwindow.h \
    chattextedit.h \
    messagelist.h \
    messageitem.h \
    signinwindow.h \
    settingswindow.h \
    listitems/contactlistitem.h \
    listitems/listitem.h \
    listitems/serviceitem.h \
    listitems/grouplistitem.h \
    speexdecoder.h \
    services/service.h \
    services/palringoservice.h \
    contactpropertieswindow.h \
    listviews/contactlistview.h \
    listviews/grouplistview.h \
    listviews/overviewlistview.h \
    services/bridgeservice.h

LIBS += -L../bin \
    -lQPalringo \
    -lssl \
    -lspeex \
    -lz
RESOURCES += ../resources/qpalringo.qrc
DISTFILES += ../qss/qpalringo.css \
    ../qss/listitem.qss \
    ../qss/messageitem.css
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
UI_DIR = ../tmp
