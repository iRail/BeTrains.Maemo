//
// Configuration
//

// Include guard
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

// Includes
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "widgets/connectionrequestwidget.h"
#include "widgets/connectionresultwidget.h"
#include "widgets/connectiondetailwidget.h"
#include "widgets/liveboardwidget.h"
#include "dialogs/optionalprogressdialog.h"
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QList>
#include <QModelIndex>
#include <QLabel>
#include <QtMaemo5/QMaemo5InformationBox>

namespace iRail
{
    class MainWindow : public QScrollArea
    {
    Q_OBJECT
    public:
        explicit MainWindow(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainWindow();

        // Public slots
    public slots:
        void load_requestwidget(QMap<QString, StationPointer>* iStations);
        void show_requestwidget();
        void process_requestwidget(ConnectionRequestPointer iConnectionRequest);

        void load_resultwidget();
        void show_resultwidget(QList<ConnectionPointer>* iConnections);
        void process_resultwidget(ConnectionPointer iConnection);

        void load_detailwidget();
        void load_detailwidget_vehicle(VehiclePointer* iVehicle);
        void show_detailwidget(ConnectionPointer iConnection);

        void load_liveboardwidget(QMap<QString, StationPointer>* iStations);
        void show_liveboardwidget();
        void process_liveboardwidget(Liveboard::Departure iDeparture);

        // UI events
    private slots:
        void load_history(QModelIndex iIndex);

    private:
        // Member data
        CachedAPI* mAPI;
        QList<ConnectionRequestPointer> mConnectionRequestHistory;

        // Temporary data
        ConnectionRequestPointer tInitialRequest;
        ConnectionPointer tConnection;
        QMap<QString, VehiclePointer> tVehicles;

        // UI members
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;
        QPushButton* mUIButtonSearch;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;
        ConnectionRequestWidget* mChildConnectionRequest;
        ConnectionResultWidget* mChildConnectionResult;
        ConnectionDetailWidget* mChildConnectionDetail;
        LiveboardWidget* mChildLiveboard;

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINWIDGET_H
