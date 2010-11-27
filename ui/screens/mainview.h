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

namespace iRail
{
    class MainView : public GenericView
    {

    Q_OBJECT
    public:
        MainView(QWidget *iParent);
        ~MainView();

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);

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
