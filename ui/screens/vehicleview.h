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
#include "genericview.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QListView>
#include <QStandardItemModel>

namespace iRail
{
    class VehicleView : public GenericView
    {

    Q_OBJECT
    public:
        VehicleView(QWidget *iParent);
        ~VehicleView();
        void load(Connection::Line iConnectionLine);
        void load(const QMap<QString, StationPointer>& iStations, Connection::Line iLine, VehiclePointer iVehicle);

        // Temporary data
        Connection::Line tLine;
        VehiclePointer tVehicle;

        // UI events
    private slots:

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setVehicle(VehiclePointer* iVehicles);

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
        void populateModel(Connection::Line iLine, VehiclePointer iVehicle);

        // UI members
    private:
        QScrollArea* mUIScrollArea;
        QVBoxLayout* mUIScrollLayout;
        QListView* mView;
        QStandardItemModel* mModel;
    };
}

#endif // VEHICLEVIEW_H
