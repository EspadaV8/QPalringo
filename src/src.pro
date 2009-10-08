SOURCES += contactpropertieswindow.cpp \
    main.cpp \
    palringowindow.cpp \
    settingswindow.cpp
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
    debug
TARGET = ../bin/qpalringo
FORMS = ui/contactProperties.ui \
    ui/settingswindow.ui
QT += gui \
    svg \
    network
HEADERS += contactpropertieswindow.h \
    palringowindow.h \
    settingswindow.h
LIBS += -L../bin \
    -lqpcore \
    -lssl \
    -lz
RESOURCES += ../resources/qpalringo.qrc
DISTFILES += ../qss/qpalringo.css \
    ../qss/listitem.qss \
    ../qss/messageitem.css
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
UI_DIR = ../tmp
INCLUDEPATH = ../libqpcore \
    ../libQPalringo
QMAKE_LFLAGS += -Wl,-rpath=. -Wl,-rpath-link=../bin

# SOURCES += speexdecoder.cpp
# HEADERS += speexdecoder.h
# LIBS += -lspeex

