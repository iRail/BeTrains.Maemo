//
// Configuration
//

// Include guard
#ifndef VEHICLEVIEW_H
#define VEHICLEVIEW_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "api/vehicle.h"
#include "api/liveboard.h"
#include <QVBoxLayout>
#include <QScrollArea>

namespace iRail
{
    class VehicleView : public QWidget
    {

    Q_OBJECT
    public:
        VehicleView(QWidget *iParent);
        ~VehicleView();
        void showUI(ConnectionPointer iConnection);
        void load(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles);

        // Auxiliary types
        QMap<QString, StationPointer>* tStations;
        QMap<QString, VehiclePointer>* tVehicles;
        ConnectionPointer tConnection;

        // UI events
    private slots:
        void _showConnectionDetail(ConnectionPointer iConnection);
        void _showConnectionDetail(const QMap<QString, StationPointer>& iStations, ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setVehicle(VehiclePointer* iVehicle);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void downloadVehicle(QString iVehicleId);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
    private:
        void init_ui();
        void update_ui(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles);
        void init_children();
        void init_line(const Connection::Line& iLine, const VehiclePointer& iVehicle);

        // UI members
    private:
        QVBoxLayout* mUIScrollLayout;
    };
}

#endif // VEHICLEVIEW_H
