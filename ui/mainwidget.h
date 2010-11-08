//
// Configuration
//

// Include guard
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

// Includes
#include <QWidget>
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "connectionrequestwidget.h"
#include <QListView>
#include <QStandardItem>
#include <QList>
#include "ui/global.h"
#include <QModelIndex>
#include <QLabel>

namespace iRail
{
    class MainWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit MainWidget(CachedAPI* iAPI, QWidget *parent = 0);

    public slots:
        void show_connectionquerywidget();
        void add_history(ConnectionRequestPointer iConnectionRequest);
        void load_history(QModelIndex iIndex);

    private:
        // Member data
        CachedAPI* mAPI;
        QList<ConnectionRequestPointer> mConnectionRequestHistory;

        // UI members
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;

        // Initialization
        void init_ui();
        void init_children();

        // Child widgets
        ConnectionRequestWidget* mChildConnectionRequest;

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINWIDGET_H
