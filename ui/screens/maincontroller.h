//
// Configuration
//

// Include guard
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

// Includes
#include <QDebug>
#include "ui/global.h"
#include "cachedapi.h"
#include "api/connectionrequest.h"
#include "mainview.h"
#include <QList>
#include <QMap>
#include "liveboardcontroller.h"
#include "connectioncontroller.h"


namespace iRail
{
    class MainController : public QObject
    {
    Q_OBJECT
    public:
        explicit MainController(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainController();
        void showView();

        // View slots
    private slots:
        void _downloadStations();
        void _launchConnection(ConnectionRequestPointer iConnectionRequest);
        void _launchLiveboard();

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;

        // Screens
        LiveboardController* mScreenLiveboard;
        ConnectionController* mScreenConnection;
    };
}

#endif // MAINCONTROLLER_H
