SOURCES += DataMap.cc \
 MiscFunctions.cc \
 callbackFunctions.cc \
 crypto.cc \
 libPalringo.cc \
 md5.c \
 my_oldpassword.c \
 salsa20.c

TEMPLATE = lib
CONFIG += warn_on debug staticlib
TARGET = Palringo

QT -= core gui

HEADERS += DataMap.h \
 MiscFunctions.h \
 crypto.h \
 ecrypt-config.h \
 ecrypt-machine.h \
 ecrypt-portable.h \
 libPalringo.h \
 md5.h \
 my_oldpassword.h \
 salsa20.h

MOC_DIR     = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp
