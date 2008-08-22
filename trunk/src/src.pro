SOURCES += main.cpp \
 palringowindow.cpp \
 palringolistview.cpp \
 contactlistitem.cpp \
 listviewcontainer.cpp \
 listitem.cpp \
 serviceitem.cpp \
 tools.cpp \
 chatwindow.cpp \
 messagelist.cpp \
 messageitem.cpp \
 qpalringoconnection.cpp \
 signinwindow.cpp \
 connection.cpp \
 contact.cpp \
 group.cpp \
 grouplistitem.cpp \
 target.cpp

TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
          debug
TARGET = ../bin/qpalringo

FORMS = ui/signinwindow.ui

QT += gui
HEADERS += palringowindow.h \
 palringolistview.h \
 contactlistitem.h \
 listviewcontainer.h \
 listitem.h \
 serviceitem.h \
 tools.h \
 chatwindow.h \
 messagelist.h \
 messageitem.h \
 qpalringoconnection.h \
 signinwindow.h \
 connection.h \
 contact.h \
 group.h \
 grouplistitem.h \
 target.h

LIBS += -LlibPalringo -lPalringo -lssl

RESOURCES += ../resources/qpalringo.qrc

DISTFILES += ../qss/qpalringo.css \
 ../qss/listitem.qss \
 ../qss/messageitem.css

MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
UI_DIR = ../tmp
