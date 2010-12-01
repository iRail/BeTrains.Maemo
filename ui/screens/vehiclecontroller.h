//
// Configuration
//

// Include guard
#ifndef VEHICLECONTROLLER_H
#define VEHICLECONTROLLER_H

// Includes
#include <QDebug>
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connection.h"
#include "vehicleview.h"
#include <QList>
#include <QMap>


namespace iRail
{
    class VehicleController : public QObject
    {
    Q_OBJECT
    public:
        explicit VehicleController(CachedAPI* iAPI, QWidget *parent = 0);
        ~VehicleController();
        void showView(Connection::Line iConnectionLine);

        // View slots
    private slots:
        void _downloadStations();
        void _downloadVehicle(QString iVehicleId);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);
        void gotVehicle(VehiclePointer* iVehicle, QDateTime iTimestamp);

    private:
        // Member data
        CachedAPI* mAPI;
        VehicleView* mView;
    };
}

#endif // VEHICLECONTROLLER_H
