//
// Configuration
//

// Include guard
#ifndef CONNECTIONRESULTWIDGET_H
#define CONNECTIONRESULTWIDGET_H

// Includes
#include <QWidget>
#include "api/connectionrequest.h"
#include "cachedapi.h"
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
        explicit ConnectionResultWidget(CachedAPI* iAPI, QWidget *iParent);
        void load(const ConnectionRequestPointer& iConnectionRequest);
        void load(const QList<ConnectionPointer>& iConnections);

        // UI events
    private slots:
        void activated(QModelIndex iIndex);

    signals:
        void finished(ConnectionPointer iConnection);

    private:
        // Member data
        CachedAPI* mAPI;

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
