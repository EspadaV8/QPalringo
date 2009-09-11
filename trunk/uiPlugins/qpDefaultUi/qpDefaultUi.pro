# -------------------------------------------------
# Project created by QtCreator 2009-09-09T20:42:14
# -------------------------------------------------
QT += svg network
TARGET = ../../bin/$$qtLibraryTarget(qpdefaultui)
TEMPLATE = lib
CONFIG += plugin \
    debug
SOURCES += qpdefaultui.cpp \
    listviews/palringolistview.cpp \
    listviews/overviewlistview.cpp \
    listviews/listviewcontainer.cpp \
    listviews/grouplistview.cpp \
    listviews/defaultlistviewcontainer.cpp \
    listviews/contactlistview.cpp \
    listitems/targetlistitem.cpp \
    listitems/serviceitem.cpp \
    listitems/listitem.cpp \
    listitems/grouplistitem.cpp \
    listitems/contactlistitem.cpp \
    listitems/bridgecontactlistitem.cpp \
    services/service.cpp \
    services/palringoservice.cpp \
    services/bridgeservice.cpp \
    chatwindow/chatwindow.cpp \
    chatwindow/chattextedit.cpp
HEADERS += qpdefaultui.h \
    listviews/palringolistview.h \
    listviews/overviewlistview.h \
    listviews/listviewcontainer.h \
    listviews/grouplistview.h \
    listviews/defaultlistviewcontainer.h \
    listviews/contactlistview.h \
    listitems/targetlistitem.h \
    listitems/serviceitem.h \
    listitems/listitem.h \
    listitems/grouplistitem.h \
    listitems/contactlistitem.h \
    listitems/bridgecontactlistitem.h \
    services/service.h \
    services/palringoservice.h \
    services/bridgeservice.h \
    chatwindow/chatwindow.h \
    chatwindow/chattextedit.h
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
INCLUDEPATH = ../../libQPalringo \
    ../../src
