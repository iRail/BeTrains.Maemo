//
// Configuration
//

// Include guard
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

// Includes
#include <QDebug>
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "mainview.h"
#include <QList>
#include <QMap>
#include "liveboardcontroller.h"
#include "vehiclecontroller.h"


namespace iRail
{
    class MainController : public QObject
    {
    Q_OBJECT
    public:
        explicit MainController(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainController();
        void showView();

        // View slots
    private slots:
        void _downloadStations();
        void _downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void _launchVehicle(ConnectionPointer iConnection);
        void _launchLiveboard();

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations);
        void gotConnections(QList<ConnectionPointer>* iConnections);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;

        // Screens
        LiveboardController* mScreenLiveboard;
        VehicleController* mScreenVehicle;
    };
}

#endif // MAINCONTROLLER_H
