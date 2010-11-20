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
        void showView();

        // View slots
    private slots:
        void _downloadStations();
        void _downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void _downloadVehicle(QString iVehicleId);
        void _downloadLiveboard(QString iStationId);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations);
        void gotConnection(QList<ConnectionPointer>* iConnections);
        void gotVehicle(VehiclePointer* iVehicle);
        void gotLiveboard(LiveboardPointer* iLiveboard);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;

        // Temporary data
        ConnectionPointer tConnection;
        QMap<QString, VehiclePointer> tVehicles;
    };
}

#endif // MAINCONTROLLER_H
