# -------------------------------------------------
# Project created by QtCreator 2009-04-25T10:47:06
# -------------------------------------------------
TEMPLATE = lib
CONFIG += plugin static
INCLUDEPATH += ../../src \
    ../../libQPalringo
HEADERS += qpdefaultui.h \
    contactpropertieswindow.h \
    listviews/contactlistview.h \
    listviews/grouplistview.h \
    listviews/overviewlistview.h \
    listviews/palringolistview.h \
    listviews/listviewcontainer.h \
    uitools.h
SOURCES += qpdefaultui.cpp \
    contactpropertieswindow.cpp \
    listviews/contactlistview.cpp \
    listviews/grouplistview.cpp \
    listviews/overviewlistview.cpp \
    listviews/palringolistview.cpp \
    listviews/listviewcontainer.cpp \
    uitools.cpp
FORMS = ui/contactProperties.ui
TARGET = $$qtLibraryTarget(qp_default_ui)
DESTDIR = ../../bin
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
UI_DIR = ../../tmp
QT += gui \
    svg \
    network
