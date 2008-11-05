SOURCES += main.cpp \
 palringowindow.cpp \
 listviews/palringolistview.cpp \
 listviews/listviewcontainer.cpp \
 tools.cpp \
 chatwindow.cpp \
 messagelist.cpp \
 messageitem.cpp \
 qpalringoconnection.cpp \
 signinwindow.cpp \
 connection.cpp \
 targets/contact.cpp \
 targets/group.cpp \
 targets/target.cpp \
 listitems/contactlistitem.cpp \
 listitems/listitem.cpp \
 listitems/serviceitem.cpp \
 listitems/grouplistitem.cpp \
 speexdecoder.cpp \
 service.cpp \
 palringoservice.cpp \
 contactpropertieswindow.cpp \
 listviews/contactlistview.cpp \
 listviews/grouplistview.cpp

TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
          debug
TARGET = ../bin/qpalringo

FORMS = ui/signinwindow.ui \
 ui/contactProperties.ui

QT += gui phonon svg
HEADERS += palringowindow.h \
 listviews/palringolistview.h \
 listviews/listviewcontainer.h \
 tools.h \
 chatwindow.h \
 messagelist.h \
 messageitem.h \
 qpalringoconnection.h \
 signinwindow.h \
 connection.h \
 targets/contact.h \
 targets/group.h \
 targets/target.h \
 listitems/contactlistitem.h \
 listitems/listitem.h \
 listitems/serviceitem.h \
 listitems/grouplistitem.h \
 message.h \
 speexdecoder.h \
 service.h \
 palringoservice.h \
 contactpropertieswindow.h \
 listviews/contactlistview.h \
 listviews/grouplistview.h

LIBS += -LlibPalringo -lPalringo -lssl -lspeex -lz

RESOURCES += ../resources/qpalringo.qrc

DISTFILES += ../qss/qpalringo.css \
 ../qss/listitem.qss \
 ../qss/messageitem.css

MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
UI_DIR = ../tmp
