//
// Configuration
//

// Include guard
#ifndef CONNECTIONRESULTWIDGET_H
#define CONNECTIONRESULTWIDGET_H

// Includes
#include <QWidget>
#include "api/station.h"
#include "api/connection.h"
#include "api/connectionrequest.h"
#include <QListView>
#include <QStandardItem>
#include "ui/global.h"
#include <QModelIndex>

namespace iRail
{
    class ConnectionResultWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionResultWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent);
        ~ConnectionResultWidget();
        void load(const QList<ConnectionPointer>& iConnections);

        // UI events
    private slots:
        void activated(QModelIndex iIndex);

    signals:
        void finished(ConnectionPointer iConnection);

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

#endif // CONNECTIONRESULTWIDGET_H
