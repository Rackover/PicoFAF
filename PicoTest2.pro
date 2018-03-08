QT -= gui

CONFIG += c++1z console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    authentification.cpp \
    games.cpp \
    logging.cpp \
    reaction.cpp \
    serverlink.cpp \
    settings.cpp \
    display.cpp

HEADERS += \
    utils.h \
    settings.h \
    authentification.h \
    games.h \
    logging.h \
    reaction.h \
    serverlink.h \
    display.h

#CUSTOM


#PDC
INCLUDEPATH += C:\Qt\custom-libs\pdcurses
LIBS += -L"C:\Qt\custom-libs\pdcurses\wincon\build" -lpdcurses
#ENDOf


QT += network
QT += core

OTHER_FILES +=

OBJECTS_DIR = BUILD/.obj
MOC_DIR = BUILD/.moc
RCC_DIR = BUILD/.rcc
DESTDIR = BIN

DISTFILES += \
    external/uid-generator.exe \
    config.txt
