# -------------------------------------------------
# Project created by QtCreator 2009-04-12T12:41:59
# -------------------------------------------------
QT += network
QT -= gui

TARGET = QPalringo

TEMPLATE = lib

CONFIG += warn_on debug staticlib

DEFINES += LIBQPALRINGO_LIBRARY

SOURCES += qpalringoconnection.cpp \
    targets/target.cpp \
    targets/contact.cpp \
    targets/group.cpp \
    message.cpp

HEADERS += qpalringoconnection.h \
    targets/target.h \
    targets/contact.h \
    targets/group.h \
    messagedata.h \
    message.h \
    user.h

MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
