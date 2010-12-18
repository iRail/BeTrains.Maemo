//
// Configuration
//

// Include guard
#ifndef LIVEBOARDVIEWIMPL_H
#define LIVEBOARDVIEWIMPL_H

// Inclused
#include <QDebug>
#include "ui/liveboardview.h"
#include "genericviewimpl.h"
#include "api/vehicle.h"
#include "api/liveboardrequest.h"
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
    class LiveboardViewImpl : public GenericViewImpl, public LiveboardView
    {
    Q_OBJECT
    public:
        LiveboardViewImpl();

        // UI events
    private slots:
        void do_btnStations_clicked();
        void do_lstDepartures_activated(QModelIndex iIndex);
        void do_btnMore_clicked();

        // Controller actions
    public slots:
        void reset();
        void load();
        void load(LiveboardRequestPointer iLiveboardRequest);
        void setStations(QMap<QString, StationPointer>* iStations);
        void setLiveboard(LiveboardPointer* iLiveboard);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;
        QList<Liveboard::Departure> mDepartures;
        LiveboardRequestPointer tLiveboardRequest;

        // UI Members
        QLineEdit *mUIStationEdit;
        QPushButton *mUIStationButton;
        QLabel *mViewDummy;
        QWidget *mViewHeader;
        QListView *mView;
        QStandardItemModel *mModel;

        // Initialization
        void init_ui();

        // Auxiliary
        void populateModel();
    };
}

#endif // LIVEBOARDVIEWIMPL_H
