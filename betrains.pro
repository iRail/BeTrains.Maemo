
#
# Base configuration
#

TARGET=BETrains

DEFINES += BETRAINS_UI_MAEMO BETRAINS_UI_ID=Maemo BETRAINS_UI_NAME=\\\"Maemo\\\" BETRAINS_UI_RES=maemo
include(../libirail/libirail.pri)

QT       += gui maemo5

RESOURCES += ui.qrc \
    translations.qrc
SOURCES += main.cpp \
    ui.cpp \
    ui/mainwidget.cpp \
    ui/connectionrequestwidget.cpp \
    ui/connectionresultwidget.cpp \
    ui/dialogs/stationchooser.cpp \
    ui/dialogs/progressindicator.cpp
HEADERS += \
    ui.h \
    ui/global.h \
    ui/mainwidget.h \
    ui/connectionrequestwidget.h \
    ui/connectionresultwidget.h \
    ui/dialogs/stationchooser.h \
    ui/dialogs/progressindicator.h
TRANSLATIONS += \
    translations/nl_BE.ts



unix:!symbian {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    # VARIABLES
    BINDIR = $$PREFIX/bin
    DATADIR =$$PREFIX/share
    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    INSTALLS += target desktop icon48 icon64

    target.path = $$BINDIR

    desktop.path = $$DATADIR/applications/hildon
    desktop.files += qtc_packaging/$${TARGET}.desktop

    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon48.files += ../../data/icons/hicolor/48x48/$${TARGET}.png

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += ../../data/icons/hicolor/64x64/$${TARGET}.png
}
