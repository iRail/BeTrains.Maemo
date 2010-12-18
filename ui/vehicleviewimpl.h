//
// Configuration
//

// Include guard
#ifndef VEHICLEVIEWIMPL_H
#define VEHICLEVIEWIMPL_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "api/vehicle.h"
#include "api/liveboard.h"
#include "ui/vehicleview.h"
#include "genericviewimpl.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QListView>
#include <QStandardItemModel>

namespace iRail
{
    class VehicleViewImpl : public VehicleView
    {

    Q_OBJECT
    public:
        VehicleViewImpl();
        ~VehicleViewImpl();

        // Temporary data
        Connection::Line tLine;
        VehiclePointer tVehicle;

        // UI events
    private slots:

        // Controller actions
    public slots:
        void reset();
        void load(Connection::Line iConnectionLine);
        void setStations(QMap<QString, StationPointer>* iStations);
        void setVehicle(VehiclePointer* iVehicles);

        // GenericView proxy
        void showError(const QString& iError)
        { proxy->showError(iError); }
        void startLoader()
        { proxy->startLoader(); }
        void stopLoader()
        { proxy->stopLoader(); }
        void showProgress()
        { proxy->showProgress(); }

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
    private:
        void init_ui();
        void populateModel(Connection::Line iLine, VehiclePointer iVehicle);

        // UI members
    private:
        QListView* mView;
        QStandardItemModel* mModel;

        // Proxy object
        GenericViewImpl* proxy;
    };
}

#endif // VEHICLEVIEWIMPL_H
