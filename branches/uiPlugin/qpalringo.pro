SUBDIRS += src/libPalringo \
    src/libQPalringo \
    src
TEMPLATE = subdirs
CONFIG += warn_on \
    qt \
    thread \
    debug
MOC_DIR = tmp
OBJECTS_DIR = tmp