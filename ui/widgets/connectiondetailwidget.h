//
// Configuration
//

// Include guard
#ifndef CONNECTIONDETAILWIDGET_H
#define CONNECTIONDETAILWIDGET_H

// Includes
#include "ui/global.h"
#include "api/station.h"
#include "api/connection.h"
#include "api/vehicle.h"
#include <QMap>
#include <QVBoxLayout>
#include <QScrollArea>

namespace iRail
{
    class ConnectionDetailWidget : public QScrollArea
    {
    Q_OBJECT
    public:
        explicit ConnectionDetailWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent);
        ~ConnectionDetailWidget();
        void load(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles);

    private:
        // Member data
        const QMap<QString, StationPointer> mStations;

        // Initialization
    private:
        void init_ui();
        void update_ui(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles);
        void init_children();
        void init_line(const Connection::Line& iLine, const VehiclePointer& iVehicle);

        // UI members
    private:
        QVBoxLayout* mUILayout;
    };
}

#endif // CONNECTIONDETAILWIDGET_H
