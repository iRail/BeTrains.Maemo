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


namespace iRail
{
    class MainController : public QScrollArea
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
        void _downloadVehicle(QString iVehicleId);
        void _launchLiveboard();

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations);
        void gotConnections(QList<ConnectionPointer>* iConnections);
        void gotVehicle(VehiclePointer* iVehicle);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;

        // Screens
        LiveboardController* mScreenLiveboard;
    };
}

#endif // MAINCONTROLLER_H
