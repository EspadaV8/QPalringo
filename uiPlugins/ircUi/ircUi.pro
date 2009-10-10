# -------------------------------------------------
# Project created by QtCreator 2009-10-06T22:17:22
# -------------------------------------------------
QT += svg \
    network
TARGET = ../../bin/$$qtLibraryTarget(ircui)
TEMPLATE = lib
CONFIG += plugin \
    debug
SOURCES += ircui.cpp \
    chatview.cpp \
    accountlist.cpp \
    panes/qpalpane.cpp \
    panes/overviewpane.cpp \
    panes/palringoservicepane.cpp \
    panes/targetpanes/targetpane.cpp \
    panes/targetpanes/grouppane.cpp
HEADERS += ircui.h \
    chatview.h \
    accountlist.h \
    panes/qpalpane.h \
    panes/overviewpane.h \
    panes/palringoservicepane.h \
    panes/targetpanes/targetpane.h \
    panes/targetpanes/grouppane.h
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
UI_DIR = ../../tmp
INCLUDEPATH = ../../libQPalringo \
    ../../libqpcore \
    ../../src \
    ../qpDefaultUi
LIBS += -L../../bin \
    -lqpcore \
    -lqpdefaultui
QMAKE_LFLAGS += -Wl,-rpath=. \
    -Wl,-rpath-link=../../bin
