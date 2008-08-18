SOURCES += DataMap.cc \
 MiscFunctions.cc \
 callbackFunctions.cc \
 crypto.cc \
 libPalringo.cc

TEMPLATE = lib
CONFIG += warn_on debug
TARGET = libPalringo.a

QT = -core -gui

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
