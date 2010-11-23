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
#include "ui/dialogs/optionalprogressdialog.h"
#include "genericview.h"

namespace iRail
{
    class MainView : public QScrollArea
    {

    Q_OBJECT
    public:
        MainView(QWidget *iParent);
        ~MainView();
        void showUI();

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void launchLiveboard();
        void launchRequest();
        void setInitialRequest(ConnectionRequestPointer iInitialRequest);

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

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINVIEW_H
