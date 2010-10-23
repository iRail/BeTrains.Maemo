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
#include <QProgressDialog>

namespace iRail
{
    class ConnectionResultWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionResultWidget(CachedAPI* iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent);

        // UI slots
    private slots:
        void show_progressdialog();
        void show_connections(const QList<ConnectionPointer>& iConnections);

    private:
        // Member data
        CachedAPI* mAPI;
        ConnectionRequestPointer mConnectionRequest;
        const QList<ConnectionPointer>* mConnections;    // TODO: who takes ownership, etc

        // Initialization
    private:
        void init_ui();
        void init_children();

        // UI members
    private:
        QListView *mView;
        QStandardItemModel *mModel;
        QProgressDialog* mUIProgressDialog;
        QString* mUIProgressMessage;

        // Auxiliary
        void populateModel();
    };
}

#endif // CONNECTIONRESULTWIDGET_H
