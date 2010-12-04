
#
# Base configuration
#

TARGET=BeTrains

include(../libirail/libirail.pri)

QT       += gui maemo5

RESOURCES += ui.qrc \
    translations.qrc
SOURCES += main.cpp \
    ui/screens/maincontroller.cpp \
    ui/dialogs/stationchooser.cpp \
    ui/auxiliary/delegates/connectiondelegate.cpp \
    ui/auxiliary/delegates/connectionrequestdelegate.cpp \
    ui/auxiliary/delegates/connectionpoidelegate.cpp \
    ui/auxiliary/delegates/vehiclestopdelegate.cpp \
    ui/auxiliary/delegates/liveboarddeparturedelegate.cpp \
    ui/screens/mainview.cpp \
    ui/screens/liveboardcontroller.cpp \
    ui/screens/liveboardview.cpp \
    ui/screens/vehicleview.cpp \
    ui/screens/vehiclecontroller.cpp \
    ui/screens/connectioncontroller.cpp \
    ui/screens/connectionview.cpp \
    ui/screens/requestcontroller.cpp \
    ui/screens/requestview.cpp \
    ui/auxiliary/loaderwidget.cpp \
    ui/screens/genericview.cpp \
    application.cpp \
    ui/dialogs/about.cpp
HEADERS += \
    ui/global.h \
    ui/screens/maincontroller.h \
    ui/dialogs/stationchooser.h \
    ui/auxiliary/delegates/connectiondelegate.h \
    ui/auxiliary/delegates/connectionrequestdelegate.h \
    ui/auxiliary/delegates/connectionpoidelegate.h \
    ui/auxiliary/delegates/vehiclestopdelegate.h \
    ui/auxiliary/delegates/liveboarddeparturedelegate.h \
    ui/screens/mainview.h \
    ui/screens/liveboardcontroller.h \
    ui/screens/liveboardview.h \
    ui/screens/vehicleview.h \
    ui/screens/vehiclecontroller.h \
    ui/screens/connectioncontroller.h \
    ui/screens/connectionview.h \
    ui/screens/requestcontroller.h \
    ui/screens/requestview.h \
    ui/auxiliary/loaderwidget.h \
    ui/screens/genericview.h \
    ui/auxiliary/animationwidget.h \
    ui/auxiliary/animationrectitem.h \
    ui/auxiliary/graphicsrectobject.h \
    application.h \
    ui/dialogs/about.h
TRANSLATIONS += \
    translations/nl_BE.ts

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
    desktop.files += qtc_packaging/$${TARGET}.desktop

    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon48.files += ../../data/icons/hicolor/48x48/$${TARGET}.png

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += ../../data/icons/hicolor/64x64/$${TARGET}.png
}
