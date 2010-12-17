
#
# Base configuration
#

TARGET=BeTrains

include(../libirail/libirail.pri)
include(../alpha/alpha.pri)

QT       += gui maemo5

RESOURCES += ui.qrc \
    i18n_maemo.qrc
TRANSLATIONS += \
    i18n_maemo/nl_BE.ts \
    i18n_maemo/en_US.ts
SOURCES += main.cpp 
HEADERS += 

# This conditional needed to trick Qt Creator
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
    desktop.files += $${TARGET}.desktop

    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon48.files += ../../data/icons/hicolor/48x48/$${TARGET}.png

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += ../../data/icons/hicolor/64x64/$${TARGET}.png
}
