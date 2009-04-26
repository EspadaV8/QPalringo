SOURCES += main.cpp \
    palringowindow.cpp \
    tools.cpp \
    settingswindow.cpp \
    speexdecoder.cpp
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
    debug
TARGET = ../bin/qpalringo
FORMS = ui/settingswindow.ui
QT += gui \
    svg \
    network
HEADERS += palringowindow.h \
    tools.h \
    settingswindow.h \
    speexdecoder.h \
    qpuiplugin.h
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
