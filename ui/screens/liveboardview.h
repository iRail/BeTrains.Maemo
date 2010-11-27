//
// Configuration
//

// Include guard
#ifndef LIVEBOARDVIEW_H
#define LIVEBOARDVIEW_H

// Inclused
#include <QDebug>
#include "genericview.h"
#include "api/vehicle.h"
#include "api/liveboard.h"
#include <QVBoxLayout>
#include <QModelIndex>
#include <QLineEdit>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>
#include <QPushButton>

namespace iRail
{
    class LiveboardView : public GenericView
    {

    Q_OBJECT
    public:
        LiveboardView(QWidget *iParent);
        ~LiveboardView();
        void showUI();
        void load(LiveboardPointer iLiveboard);

        // Temporary data
        LiveboardPointer tLiveboard;

        // UI events
    private slots:
        void _showLiveboardRequest();
        void _showLiveboardRequest(const QMap<QString, StationPointer>& iStations);
        void _showLiveboardResult(QString iStationId);
        void _showLiveboardResult(LiveboardPointer iLiveboard);
        void do_search();
        void do_stations();
        void do_detail(QModelIndex iIndex);
        void clear();

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setVehicle(VehiclePointer* iVehicle);
        void setLiveboard(LiveboardPointer* iLiveboard);

        // Controller signals
    signals:
        void downloadStations();
        void downloadVehicle(QString iVehicleId);
        void downloadLiveboard(QString iStationId);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;
        QString tStationId;

        // UI Members
        QLineEdit *mUIStationEdit;
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;
        QPushButton *mUIStationButton;

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel(const QList<Liveboard::Departure>& iDepartures);
    };
}

#endif // LIVEBOARDVIEW_H
