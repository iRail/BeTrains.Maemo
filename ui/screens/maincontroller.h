//
// Configuration
//

// Include guard
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

// Includes
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "mainview.h"
#include "ui/widgets/connectionrequestwidget.h"
#include "ui/widgets/connectionresultwidget.h"
#include "ui/widgets/connectiondetailwidget.h"
#include "ui/widgets/liveboardwidget.h"
#include "ui/dialogs/optionalprogressdialog.h"
#include <QList>


namespace iRail
{
    class MainController : public QScrollArea
    {
    Q_OBJECT
    public:
        explicit MainController(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainController();

        // Public slots
    public slots:
        void load_stations(QObject* iObject, const char* iSlot);
        void process_stations(QMap<QString, StationPointer>* iStations);
        void disconnect_stations(QObject* iObject, const char* iSlot);

        void load_connections(ConnectionRequestPointer iConnectionRequest, QObject* iObject, const char* iSlot);
        void process_connections(QList<ConnectionPointer>* iStations);
        void disconnect_connections(QObject* iObject, const char* iSlot);

        void load_connectionrequestwidget(QMap<QString, StationPointer>* iStations);
        void show_connectionrequestwidget();
        void process_connectionrequestwidget(ConnectionRequestPointer iConnectionRequest);

        void load_vehicle(QString iVehicleId, QObject* iObject, const char* iSlot);
        void process_vehicle(VehiclePointer* iVehicle);
        void disconnect_vehicle(QObject* iObject, const char* iSlot);

        void load_liveboard(QString iStationId, QObject* iObject, const char* iSlot);
        void process_liveboard(LiveboardPointer* iLiveboard);
        void disconnect_liveboard(QObject* iObject, const char* iSlot);

        void load_connectionresultwidget();
        void show_connectionresultwidget(QList<ConnectionPointer>* iConnections);
        void process_connectionresultwidget(ConnectionPointer iConnection);

        void load_connectiondetailwidget();
        void load_connectiondetailwidget_vehicle(VehiclePointer* iVehicle);
        void show_connectiondetailwidget(ConnectionPointer iConnection);

        void load_liveboardwidget(QMap<QString, StationPointer>* iStations);
        void load_liveboardwidget_liveboard(LiveboardPointer* iLiveboard);
        void show_liveboardwidget();
        void process_liveboardwidget_station(QString iStationId);
        void process_liveboardwidget(Liveboard::Departure iDeparture);

        // Internal signals:
    signals:
        void reply_stations(QMap<QString, StationPointer>* iStations);
        void reply_connections(QList<ConnectionPointer>* iStations);
        void reply_vehicle(VehiclePointer* iVehicle);
        void reply_liveboard(LiveboardPointer* iLiveboard);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;
        QList<ConnectionRequestPointer> mConnectionRequestHistory;

        // Temporary data
        ConnectionRequestPointer tInitialRequest;
        ConnectionPointer tConnection;
        QMap<QString, VehiclePointer> tVehicles;
    };
}

#endif // MAINCONTROLLER_H
