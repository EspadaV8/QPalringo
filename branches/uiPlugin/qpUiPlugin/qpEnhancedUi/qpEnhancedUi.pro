# -------------------------------------------------
# Project created by QtCreator 2009-04-25T10:47:06
# -------------------------------------------------
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../src \
    ../../libQPalringo
HEADERS += qpenhancedui.h \
    contactpropertieswindow.h \
    uitools.h
SOURCES += qpenhancedui.cpp \
    contactpropertieswindow.cpp \
    uitools.cpp
FORMS = ui/contactProperties.ui
TARGET = $$qtLibraryTarget(qp_enhanced_ui)
DESTDIR = ../../bin
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
UI_DIR = ../../tmp
QT += gui \
    svg \
    network
