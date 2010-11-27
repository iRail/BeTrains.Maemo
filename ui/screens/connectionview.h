//
// Configuration
//

// Include guard
#ifndef ConnectionView_H
#define ConnectionView_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include <QVBoxLayout>
#include <QModelIndex>
#include <QListView>
#include <QStandardItemModel>
#include "genericview.h"

namespace iRail
{
    class ConnectionView : public GenericView
    {

    Q_OBJECT
    public:
        ConnectionView(QWidget *iParent);
        ~ConnectionView();
        void showUI(ConnectionRequestPointer iConnectionRequest);
        void load(const QList<ConnectionPointer>& iConnections);

        // Auxiliary types
        QList<ConnectionPointer>* tConnections;

        // UI events
    private slots:
        void _showConnectionResult(ConnectionRequestPointer iConnectionRequest);
        void _showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections);
        void activated(QModelIndex iIndex);

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
    private:
        void init_ui();
        void init_children();

        // UI members
    private:
        QListView *mView;
        QStandardItemModel *mModel;

        // Auxiliary
        void populateModel(const QList<ConnectionPointer>& iConnections);
    };
}

#endif // ConnectionView_H
