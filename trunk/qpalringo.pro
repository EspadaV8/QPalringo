SUBDIRS += libQPalringo \
    libqpcore \
    src \
    uiPlugins
TEMPLATE = subdirs
CONFIG += warn_on \
    qt \
    thread \
    debug
MOC_DIR = tmp
OBJECTS_DIR = tmp
