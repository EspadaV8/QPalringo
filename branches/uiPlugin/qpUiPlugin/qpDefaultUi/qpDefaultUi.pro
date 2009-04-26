# -------------------------------------------------
# Project created by QtCreator 2009-04-25T10:47:06
# -------------------------------------------------
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../src \
    ../../libQPalringo
HEADERS += qpdefaultui.h \
    signinwindow.h \
    messagelist.h \
    messageitem.h \
    contactpropertieswindow.h \
    chatwindow.h \
    chattextedit.h \
    services/service.h \
    services/palringoservice.h \
    listviews/contactlistview.h \
    listviews/grouplistview.h \
    listviews/overviewlistview.h \
    listviews/palringolistview.h \
    listviews/listviewcontainer.h \
    listitems/contactlistitem.h \
    listitems/listitem.h \
    listitems/serviceitem.h \
    listitems/grouplistitem.h

SOURCES += qpdefaultui.cpp \
    signinwindow.cpp \
    messagelist.cpp \
    messageitem.cpp \
    contactpropertieswindow.cpp \
    chatwindow.cpp \
    chattextedit.cpp \
    services/service.cpp \
    services/palringoservice.cpp \
    listviews/contactlistview.cpp \
    listviews/grouplistview.cpp \
    listviews/overviewlistview.cpp \
    listviews/palringolistview.cpp \
    listviews/listviewcontainer.cpp \
    listitems/contactlistitem.cpp \
    listitems/listitem.cpp \
    listitems/serviceitem.cpp \
    listitems/grouplistitem.cpp

FORMS = ui/signinwindow.ui \
    ui/contactProperties.ui

TARGET = $$qtLibraryTarget(qp_default_ui)
DESTDIR = ../../bin
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
UI_DIR = ../../tmp

QT += gui \
    svg \
    network
