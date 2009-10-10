SUBDIRS += qpDefaultUi ircUi
TEMPLATE = subdirs
CONFIG += warn_on \
    qt \
    thread \
    debug
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
QMAKE_LFLAGS += -Wl,-rpath=. -Wl,-rpath-link=../bin
