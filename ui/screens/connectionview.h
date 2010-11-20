//
// Configuration
//

// Include guard
#ifndef ConnectionView_H
#define ConnectionView_H

// Inclused
#include <QDebug>
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "ui/widgets/connectionresultwidget.h"
#include "ui/dialogs/optionalprogressdialog.h"
#include <QVBoxLayout>

namespace iRail
{
    class ConnectionView : public QWidget
    {

    Q_OBJECT
    public:
        ConnectionView(QWidget *iParent);
        ~ConnectionView();
        void showUI(ConnectionRequestPointer iConnectionRequest);

        // Auxiliary types
        QList<ConnectionPointer>* tConnections;

        // UI events
    private slots:
        void _showConnectionResult(ConnectionRequestPointer iConnectionRequest);
        void _showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections);

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void setConnections(QList<ConnectionPointer>* iConnections);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void downloadConnections(ConnectionRequestPointer iConnectionRequest);
        void launchVehicle(ConnectionPointer iConnection);

    private:
        // UI members
        QVBoxLayout *mUILayout;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;
        ConnectionResultWidget* mChildConnectionResult;
    };
}

#endif // ConnectionView_H
