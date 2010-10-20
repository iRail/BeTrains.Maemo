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
#include "global.h"

namespace iRail
{
    class ConnectionResultWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionResultWidget(CachedAPI* iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent);

    private:
        // Member data
        CachedAPI* mAPI;
        ConnectionRequestPointer mConnectionRequest;
        QList<ConnectionPointer> mConnections;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QListView *mView;
        QStandardItemModel *mModel;

        // Auxiliary
        void populateModel();
    };
}

#endif // CONNECTIONRESULTWIDGET_H
