//
// Configuration
//

// Include guard
#ifndef ConnectionController_H
#define ConnectionController_H

// Includes
#include <QDebug>
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "connectionview.h"
#include <QList>
#include <QMap>
#include "vehiclecontroller.h"


namespace iRail
{
    class ConnectionController : public QObject
    {
    Q_OBJECT
    public:
        explicit ConnectionController(CachedAPI* iAPI, QWidget *parent = 0);
        ~ConnectionController();
        void showView(ConnectionRequestPointer iConnectionRequest);

        // View slots
    private slots:
        void _downloadStations();
        void _downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void _launchVehicle(ConnectionPointer iConnection);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations);
        void gotConnections(QList<ConnectionPointer>* iConnections);

    private:
        // Member data
        CachedAPI* mAPI;
        ConnectionView* mView;

        // Screens
        VehicleController* mScreenVehicle;
    };
}

#endif // ConnectionController_H