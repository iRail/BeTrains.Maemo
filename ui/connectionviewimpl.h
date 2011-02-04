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
#include <QLabel>

namespace iRail
{
    class ConnectionViewImpl : public ConnectionView
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

        // GenericView proxy
        void showError(const QString& iError)
        { proxy->showError(iError); }
        void startLoader()
        { proxy->startLoader(); }
        void stopLoader()
        { proxy->stopLoader(); }
        void showProgress()
        { proxy->showProgress(); }

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
        void init_ui();

        // UI members
        QTreeView *mView;
        QStandardItemModel *mModel;
        QLabel *mViewDummy;

        // Auxiliary
        void populateModel(const QList<ConnectionPointer>& iConnections);

        // Proxy object
        GenericViewImpl* proxy;
    };
}

#endif // CONNECTIONVIEWIMPL_H
