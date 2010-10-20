
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
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}
