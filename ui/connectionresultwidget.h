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
        explicit ConnectionResultWidget(CachedAPI* iAPI, QWidget *iParent);
        ~ConnectionResultWidget();

        // Public slots
    public slots:
        void setRequest(ConnectionRequestPointer iConnectionRequest);

        // UI slots
    private slots:
        void show_connections(QList<ConnectionPointer>* iConnections);

    private:
        // Member data
        CachedAPI* mAPI;

        // Initialization
    private:
        void init_ui();
        void update_ui(ConnectionRequestPointer iConnectionRequest);
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
