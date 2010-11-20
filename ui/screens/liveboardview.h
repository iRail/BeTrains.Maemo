//
// Configuration
//

// Include guard
#ifndef LIVEBOARDVIEW_H
#define LIVEBOARDVIEW_H

// Inclused
#include <QDebug>
#include "api/vehicle.h"
#include "api/liveboard.h"
#include <QObject>
#include "ui/widgets/connectiondetailwidget.h"
#include "ui/widgets/liveboardwidget.h"
#include "ui/dialogs/optionalprogressdialog.h"

namespace iRail
{
    class LiveboardView : public QWidget
    {

    Q_OBJECT
    public:
        LiveboardView(QWidget *iParent);
        ~LiveboardView();
        void showUI();

        // Auxiliary types
        enum MainAction
        {
            LIVEBOARDREQUEST = 0,
            LIVEBOARDRESULT
        };
        MainAction mAction;
        QMap<QString, StationPointer>* tStations;
        LiveboardPointer tLiveboard;

        // UI events
    private slots:
        void _showLiveboardRequest();
        void _showLiveboardRequest(const QMap<QString, StationPointer>& iStations);
        void _showLiveboardResult(QString iStationId);
        void _showLiveboardResult(const QMap<QString, StationPointer>& iStations, LiveboardPointer iLiveboard);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setVehicle(VehiclePointer* iVehicle);
        void setLiveboard(LiveboardPointer* iLiveboard);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void downloadVehicle(QString iVehicleId);
        void downloadLiveboard(QString iStationId);

    private:
        // UI members
        QVBoxLayout *mUILayout;

        // UI children
        LiveboardWidget* mChildLiveboard;
    };
}

#endif // LIVEBOARDVIEW_H
