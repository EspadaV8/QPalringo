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
    tools.cpp
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
    uiplugininterface.h
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
INCLUDEPATH = ../libQPalringo
