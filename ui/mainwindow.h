//
// Configuration
//

// Include guard
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

// Includes
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "widgets/connectionrequestwidget.h"
#include "widgets/connectionresultwidget.h"
#include "widgets/connectiondetailwidget.h"
#include "dialogs/optionalprogressdialog.h"
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QList>
#include <QModelIndex>
#include <QLabel>
#include <QtMaemo5/QMaemo5InformationBox>

namespace iRail
{
    class MainWidget : public QScrollArea
    {
    Q_OBJECT
    public:
        explicit MainWidget(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainWidget();

        // Public slots
    public slots:
        void show_request();
        void do_result(ConnectionRequestPointer iConnectionRequest);
        void show_result(QList<ConnectionPointer>* iConnections);
        void show_detail(ConnectionPointer iConnection);

        // UI events
    private slots:
        void load_history(QModelIndex iIndex);

    private:
        // Member data
        CachedAPI* mAPI;
        QList<ConnectionRequestPointer> mConnectionRequestHistory;

        // UI members
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;
        ConnectionRequestWidget* mChildConnectionRequest;
        ConnectionResultWidget* mChildConnectionResult;
        ConnectionDetailWidget* mChildConnectionDetail;

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINWIDGET_H