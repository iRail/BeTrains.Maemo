//
// Configuration
//

// Include guard
#ifndef MAINVIEW_H
#define MAINVIEW_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "api/vehicle.h"
#include "api/liveboard.h"
#include <QObject>
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QModelIndex>
#include <QLabel>
#include "ui/widgets/connectiondetailwidget.h"
#include "ui/widgets/connectionrequestwidget.h"
#include "ui/widgets/connectionresultwidget.h"
#include "ui/dialogs/optionalprogressdialog.h"

namespace iRail
{
    class MainView : public QScrollArea
    {

    Q_OBJECT
    public:
        MainView(QWidget *iParent);
        ~MainView();
        void showUI();

        // Auxiliary types
        enum MainAction
        {
            CONNECTIONREQUEST = 0,
            CONNECTIONRESULT,
            CONNECTIONDETAIL
        };
        MainAction mAction;
        QMap<QString, StationPointer>* tStations;
        QList<ConnectionPointer>* tConnections;
        QMap<QString, VehiclePointer>* tVehicles;
        ConnectionPointer tConnection;
        ConnectionRequestPointer tInitialRequest;

        // UI events
    private slots:
        void _showConnectionRequest();
        void _showConnectionRequest(const QMap<QString, StationPointer>& iStations);
        void _showConnectionResult(ConnectionRequestPointer iConnectionRequest);
        void _showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections);
        void _showConnectionDetail(ConnectionPointer iConnection);
        void _showConnectionDetail(const QMap<QString, StationPointer>& iStations, ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setConnections(QList<ConnectionPointer>* iConnections);
        void setVehicle(VehiclePointer* iVehicle);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void downloadVehicle(QString iVehicleId);
        void downloadLiveboard(QString iStationId);
        void launchLiveboard();

        // UI events
    private slots:
        void load_history(QModelIndex iIndex);

    private:
        // Member data
        QList<ConnectionRequestPointer> mConnectionRequestHistory;

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

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINVIEW_H
