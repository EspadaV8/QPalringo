# -------------------------------------------------
# Project created by QtCreator 2009-04-12T12:41:59
# -------------------------------------------------
QT += network
QT -= gui
TARGET = ../bin/QPalringo
TEMPLATE = lib
CONFIG += warn_on \
    debug
DEFINES += LIBQPALRINGO_LIBRARY
SOURCES += qpalringoconnection.cpp \
    targets/target.cpp \
    targets/contact.cpp \
    targets/group.cpp \
    message.cpp \
    headers.cpp \
    qpdatamap.cpp \
    signals.cpp \
    qpgenericdata.cpp \
    targets/bridgecontact.cpp \
    v2insignals.cpp \
    v1insignals.cpp \
    services/service.cpp \
    services/bridge.cpp \
    services/palringoservice.cpp
HEADERS += qpalringoconnection.h \
    targets/target.h \
    targets/contact.h \
    targets/group.h \
    messagedata.h \
    message.h \
    user.h \
    headers.h \
    qpcommands.h \
    qpheaderattributes.h \
    errorcodes.h \
    what.h \
    onlinestatus.h \
    qpdatamap.h \
    qpgenericdata.h \
    devicetype.h \
    groupstatus.h \
    bridgetype.h \
    targets/bridgecontact.h \
    qpsubprofilesections.h \
    qpgroupcapabilities.h \
    services/service.h \
    services/bridge.h \
    services/palringoservice.h
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
