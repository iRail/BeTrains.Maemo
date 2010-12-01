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
#include "requestcontroller.h"


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
        void _launchLiveboard();
        void _launchRequest();
        void _setInitialRequest(ConnectionRequestPointer iInitialRequest);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;

        // Temporary data
        ConnectionRequestPointer mInitialRequest;

        // Screens
        LiveboardController* mScreenLiveboard;
        RequestController* mScreenRequest;
    };
}

#endif // MAINCONTROLLER_H
