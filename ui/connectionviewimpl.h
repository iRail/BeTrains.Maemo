//
// Configuration
//

// Include guard
#ifndef CONNECTIONVIEWIMPL_H
#define CONNECTIONVIEWIMPL_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "ui/connectionview.h"
#include "genericviewimpl.h"
#include <QVBoxLayout>
#include <QModelIndex>
#include <QTreeView>
#include <QStandardItemModel>

namespace iRail
{
    class ConnectionViewImpl : public GenericViewImpl, public ConnectionView
    {

    Q_OBJECT
    public:
        ConnectionViewImpl();

        // Temporary data
        QList<ConnectionPointer>* tConnections;

        // UI events
    private slots:
        void do_lstConnections_activated(QModelIndex iIndex);

        // Controller actions
    public slots:
        void reset();
        void load(ConnectionRequestPointer iConnectionRequest);
        void setStations(QMap<QString, StationPointer>* iStations);
        void setConnections(QList<ConnectionPointer>* iConnections);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
        void init_ui();

        // UI members
        QTreeView *mView;
        QStandardItemModel *mModel;

        // Auxiliary
        void populateModel(const QList<ConnectionPointer>& iConnections);
    };
}

#endif // CONNECTIONVIEWIMPL_H
