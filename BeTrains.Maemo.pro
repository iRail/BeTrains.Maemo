
#
# Base configuration
#

TARGET=BeTrains

QT       += gui maemo5

DEPENDPATH += $$PWD/BeTrains.Qt
INCLUDEPATH += $$PWD/BeTrains.Qt
include(BeTrains.Qt/BeTrains.Qt.pri)

RESOURCES += ui.qrc \
    i18n.qrc
TRANSLATIONS += \
    i18n/nl.ts \
    i18n/fr.ts \
    i18n/de.ts \
    i18n/en.ts
SOURCES += main.cpp \
    maemoapplication.cpp \
    ui/dialogs/about.cpp \
    ui/dialogs/stationchooser.cpp \
    ui/auxiliary/delegates/connectiondelegate.cpp \
    ui/auxiliary/delegates/connectionpoidelegate.cpp \
    ui/auxiliary/delegates/vehiclestopdelegate.cpp \
    ui/auxiliary/delegates/liveboarddeparturedelegate.cpp \
    ui/auxiliary/delegates/requestdelegate.cpp \
    ui/auxiliary/loaderwidget.cpp \
    ui/connectionviewimpl.cpp \
    ui/genericviewimpl.cpp \
    ui/liveboardviewimpl.cpp \
    ui/mainviewimpl.cpp \
    ui/requestviewimpl.cpp \
    ui/vehicleviewimpl.cpp
HEADERS += \
    maemoapplication.h \
    ui/dialogs/about.h \
    ui/dialogs/stationchooser.h \
    ui/auxiliary/delegates/connectiondelegate.h \
    ui/auxiliary/delegates/connectionpoidelegate.h \
    ui/auxiliary/delegates/vehiclestopdelegate.h \
    ui/auxiliary/delegates/liveboarddeparturedelegate.h \
    ui/auxiliary/delegates/requestdelegate.h \
    ui/auxiliary/loaderwidget.h \
    ui/auxiliary/animationwidget.h \
    ui/auxiliary/animationrectitem.h \
    ui/auxiliary/graphicsrectobject.h \
    ui/connectionviewimpl.h \
    ui/genericviewimpl.h \
    ui/liveboardviewimpl.h \
    ui/mainviewimpl.h \
    ui/requestviewimpl.h \
    ui/vehicleviewimpl.h

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
    icon48.files += icons/hicolor/48x48/$${TARGET}.png

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += icons/hicolor/64x64/$${TARGET}.png
}
