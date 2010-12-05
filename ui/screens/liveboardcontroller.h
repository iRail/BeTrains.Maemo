//
// Configuration
//

// Include guard
#ifndef LIVEBOARDCONTROLLER_H
#define LIVEBOARDCONTROLLER_H

// Includes
#include <QDebug>
#include "ui/global.h"
#include "cachedapi.h"
#include "liveboardview.h"
#include "vehiclecontroller.h"
#include <QMap>


namespace iRail
{
    class LiveboardController : public QObject
    {
    Q_OBJECT
    public:
        explicit LiveboardController(CachedAPI* iAPI, QWidget *parent = 0);
        ~LiveboardController();
        void showView();
        void showView(LiveboardRequestPointer iLiveboardRequest);

        // View slots
    private slots:
        void _downloadStations();
        void _downloadLiveboard(LiveboardRequestPointer iLiveboardRequest);
        void _launchVehicle(QString iStationId, Liveboard::Departure iLiveboardDeparture);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);
        void gotLiveboard(LiveboardPointer* iLiveboard, QDateTime iTimestamp);

        // External signals:
    signals:
        void addHistory(QVariant iRequest);

    private:
        // Member data
        CachedAPI* mAPI;
        LiveboardView* mView;

        // Screens
        VehicleController* mScreenVehicle;
    };
}

#endif // LIVEBOARDCONTROLLER_H
