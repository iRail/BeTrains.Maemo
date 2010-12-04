//
// Configuration
//

// Include guard
#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

// Includes
#include <QDebug>
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "requestview.h"
#include "connectioncontroller.h"
#include <QList>
#include <QMap>


namespace iRail
{
    class RequestController : public QObject
    {
    Q_OBJECT
    public:
        explicit RequestController(CachedAPI* iAPI, QWidget *parent = 0);
        ~RequestController();
        void showView();
        void showView(ConnectionRequestPointer iInitialRequest);

        // View slots
    private slots:
        void _downloadStations();
        void _launchConnection(ConnectionRequestPointer iConnectionRequest);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);

        // External signals:
    signals:
        void addHistory(ConnectionRequestPointer iConnectionRequest);

    private:
        // Member data
        CachedAPI* mAPI;
        RequestView* mView;

        // Screens
        ConnectionController* mScreenConnection;
    };
}

#endif // REQUESTCONTROLLER_H
