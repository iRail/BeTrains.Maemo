
#
# Base configuration
#

TARGET=BeTrains

QT       += gui maemo5

DEPENDPATH += $$PWD/BeTrains.Qt
INCLUDEPATH += $$PWD/BeTrains.Qt
include(BeTrains.Qt/BeTrains.Qt.pri)

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

# Translations
TRANSLATIONS = $$files(i18n/*.ts)
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}
isEmpty(TS_DIR):TS_DIR = i18n
TSQM.name = lrelease ${QMAKE_FILE_IN}
TSQM.input = TRANSLATIONS
TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
TSQM.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += TSQM
PRE_TARGETDEPS += compiler_TSQM_make_all

# Resources
RESOURCES += ui.qrc \
    i18n.qrc

# This conditional needed to trick Qt Creator
unix:!symbian {
    # VARIABLES
    BINDIR = /opt/$$TARGET
    DATADIR =/usr/share
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

# Packaging rules
package {
	# Add debug symbols which we can strip later using dh_strip
	QMAKE_CXXFLAGS += -g
	QMAKE_CFLAGS += -g
}
