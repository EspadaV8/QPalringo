# -------------------------------------------------
# Project created by QtCreator 2009-10-03T21:22:56
# -------------------------------------------------
QT += gui network
TARGET = ../bin/qpcore
TEMPLATE = lib
CONFIG += warn_on \
    debug
LIBS += -L../bin \
    -lQPalringo
INCLUDEPATH = ../libQPalringo
DEFINES += LIBQPCORE_LIBRARY
SOURCES += tools.cpp
HEADERS += tools.h
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
