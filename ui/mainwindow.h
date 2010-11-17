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
    class MainWindow : public QScrollArea
    {
    Q_OBJECT
    public:
        explicit MainWindow(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainWindow();

        // Public slots
    public slots:
        void load_requestwidget(QMap<QString, StationPointer>* iStations);
        void show_requestwidget();
        void process_requestwidget(ConnectionRequestPointer iConnectionRequest);
        void show_resultwidget(QList<ConnectionPointer>* iConnections);
        void show_detailwidget(ConnectionPointer iConnection);

        // UI events
    private slots:
        void do_search();
        void load_history(QModelIndex iIndex);

    private:
        // Member data
        CachedAPI* mAPI;
        QList<ConnectionRequestPointer> mConnectionRequestHistory;

        // Temporary data
        ConnectionRequestPointer tInitialRequest;

        // UI members
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;
        QPushButton* mUIButtonSearch;

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
