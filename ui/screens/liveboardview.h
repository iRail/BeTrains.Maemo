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
        void load();
        void load(const QMap<QString, StationPointer>& iStations);
        void load(QString iStationId);
        void load(LiveboardPointer iLiveboard);

        // UI events
    private slots:
        void do_btnSearch_clicked();
        void do_btnStations_clicked();
        void do_lstDepartures_doubleClicked(QModelIndex iIndex);
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

        // Temporary data
        LiveboardPointer tLiveboard;

        // UI Members
        QVBoxLayout *mUIScrollLayout;
        QScrollArea* mUIScrollArea;
        QLineEdit *mUIStationEdit;
        QLabel *mViewDummy;
        QListView *mView;
        QSpacerItem* mViewSpacer;
        QVBoxLayout *mViewLayout;
        QStandardItemModel *mModel;
        QPushButton *mUIStationButton;

        // Initialization
        void init_ui();

        // Auxiliary
        void populateModel(const QList<Liveboard::Departure>& iDepartures);
    };
}

#endif // LIVEBOARDVIEW_H
