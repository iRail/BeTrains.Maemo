//
// Configuration
//

// Include guard
#ifndef CONNECTIONVIEW_H
#define CONNECTIONVIEW_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "genericview.h"
#include <QVBoxLayout>
#include <QModelIndex>
#include <QListView>
#include <QStandardItemModel>

namespace iRail
{
    class ConnectionView : public GenericView
    {

    Q_OBJECT
    public:
        ConnectionView(QWidget *iParent);
        ~ConnectionView();
        void showUI(ConnectionRequestPointer iConnectionRequest);
        void load(ConnectionRequestPointer iConnectionRequest);
        void load(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections);

        // Temporary data
        QList<ConnectionPointer>* tConnections;

        // UI events
    private slots:
        void do_lstConnections_doubleClicked(QModelIndex iIndex);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setConnections(QList<ConnectionPointer>* iConnections);

        // Controller signals
    signals:
        void downloadStations();
        void downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void launchVehicle(ConnectionPointer iConnection);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QListView *mView;
        QStandardItemModel *mModel;

        // Auxiliary
        void populateModel(const QList<ConnectionPointer>& iConnections);
    };
}

#endif // CONNECTIONVIEW_H
