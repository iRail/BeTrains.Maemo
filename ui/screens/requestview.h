//
// Configuration
//

// Include guard
#ifndef REQUESTVIEW_H
#define REQUESTVIEW_H

// Inclused
#include <QDebug>
#include "ui/global.h"
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "api/vehicle.h"
#include "api/liveboard.h"
#include <QObject>
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QModelIndex>
#include <QLabel>
#include "ui/widgets/connectionrequestwidget.h"
#include "ui/dialogs/optionalprogressdialog.h"

namespace iRail
{
    class RequestView : public QWidget
    {

    Q_OBJECT
    public:
        RequestView(QWidget *iParent);
        ~RequestView();
        void showUI();
        void showUI(ConnectionRequestPointer iInitialRequest);

        // UI events
    private slots:
        void _showConnectionRequest();
        void _showConnectionRequest(const QMap<QString, StationPointer>& iStations);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void launchConnection(ConnectionRequestPointer iConnectionRequest);

    private:
        // UI members
        QVBoxLayout *mUILayout;

        // UI children
        ConnectionRequestWidget* mChildConnectionRequest;
    };
}

#endif // REQUESTVIEW_H
