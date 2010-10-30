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
#include "dialogs/optionalprogressdialog.h"

namespace iRail
{
    class ConnectionResultWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionResultWidget(CachedAPI* iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent);
        ~ConnectionResultWidget();

        // UI slots
    private slots:
        void show_connections(QList<ConnectionPointer>* iConnections);

    private:
        // Member data
        CachedAPI* mAPI;
        ConnectionRequestPointer mConnectionRequest;

        // Initialization
    private:
        void init_ui();
        void init_children();

        // UI members
    private:
        QListView *mView;
        QStandardItemModel *mModel;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;

        // Auxiliary
        void populateModel(QList<ConnectionPointer>* iConnections);
    };
}

#endif // CONNECTIONRESULTWIDGET_H
