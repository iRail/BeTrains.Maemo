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

        // View slots
    private slots:
        void _downloadStations();
        void _downloadVehicle(QString iVehicleId);
        void _downloadLiveboard(QString iStationId);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);
        void gotVehicle(VehiclePointer* iVehicle, QDateTime iTimestamp);
        void gotLiveboard(LiveboardPointer* iLiveboard, QDateTime iTimestamp);

    private:
        // Member data
        CachedAPI* mAPI;
        LiveboardView* mView;
    };
}

#endif // LIVEBOARDCONTROLLER_H
