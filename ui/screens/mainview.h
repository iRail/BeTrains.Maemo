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
#include "genericview.h"
#include "ui/global.h"
#include <QObject>
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QModelIndex>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace iRail
{
    class MainView : public GenericView
    {

    Q_OBJECT
    public:
        MainView(QWidget *iParent);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void load();
        void load(const QMap<QString, StationPointer>& iStations);

        // Controller signals
    signals:
        void downloadStations();
        void launchLiveboard();
        void launchRequest();
        void setInitialRequest(ConnectionRequestPointer iInitialRequest);

        // UI events
    private slots:
        void do_lstHistory_clicked(QModelIndex iIndex);
        void do_actAbout_triggered();

    private:
        // Member data
        QList<ConnectionRequestPointer> mConnectionRequestHistory;
        QMap<QString, StationPointer> mStations;

        // UI members
        QLabel *mViewDummy;
        QListView *mView;
        QWidget *mViewHeader;
        QStandardItemModel *mModel;

        // Initialization
        void init_ui();
        void init_menu();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINVIEW_H
