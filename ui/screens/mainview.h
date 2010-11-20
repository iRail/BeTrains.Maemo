//
// Configuration
//

// Include guard
#ifndef MAINVIEW_H
#define MAINVIEW_H

// Inclused
#include <QDebug>
#include "ui/global.h"
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
#include "ui/widgets/connectionrequestwidget.h"
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
            CONNECTIONREQUEST = 0
        };
        MainAction mAction;
        QMap<QString, StationPointer>* tStations;
        QList<ConnectionPointer>* tConnections;
        ConnectionPointer tConnection;
        ConnectionRequestPointer tInitialRequest;

        // UI events
    private slots:
        void _showConnectionRequest();
        void _showConnectionRequest(const QMap<QString, StationPointer>& iStations);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void launchVehicle(ConnectionPointer iConnection);
        void launchLiveboard();
        void launchConnection(ConnectionRequestPointer iConnectionRequest);

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
        // Temporary friend class to get to these data
        friend class MainController;
        OptionalProgressDialog* mChildProgressDialog;
        ConnectionRequestWidget* mChildConnectionRequest;

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINVIEW_H
