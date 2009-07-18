SOURCES += chattextedit.cpp \
    chatwindow.cpp \
    contactpropertieswindow.cpp \
    main.cpp \
    messagelist.cpp \
    messageitem.cpp \
    palringowindow.cpp \
    settingswindow.cpp \
    signinwindow.cpp \
    speexdecoder.cpp \
    tools.cpp \
    listitems/bridgecontactlistitem.cpp \
    listitems/contactlistitem.cpp \
    listitems/grouplistitem.cpp \
    listitems/listitem.cpp \
    listitems/serviceitem.cpp \
    listitems/targetlistitem.cpp \
    listviews/contactlistview.cpp \
    listviews/defaultlistviewcontainer.cpp \
    listviews/grouplistview.cpp \
    listviews/listviewcontainer.cpp \
    listviews/overviewlistview.cpp \
    listviews/palringolistview.cpp \
    services/bridgeservice.cpp \
    services/palringoservice.cpp \
    services/service.cpp
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
HEADERS += chattextedit.h \
    chatwindow.h \
    contactpropertieswindow.h \
    messagelist.h \
    messageitem.h \
    palringowindow.h \
    settingswindow.h \
    signinwindow.h \
    speexdecoder.h \
    tools.h \
    listitems/bridgecontactlistitem.h \
    listitems/contactlistitem.h \
    listitems/grouplistitem.h \
    listitems/listitem.h \
    listitems/serviceitem.h \
    listitems/targetlistitem.h \
    listviews/contactlistview.h \
    listviews/defaultlistviewcontainer.h \
    listviews/grouplistview.h \
    listviews/listviewcontainer.h \
    listviews/overviewlistview.h \
    listviews/palringolistview.h \
    services/bridgeservice.h \
    services/palringoservice.h \
    services/service.h
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
