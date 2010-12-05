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
        void _launchLiveboard(LiveboardRequestPointer iLiveboardRequest);
        void _launchRequest();
        void _launchRequest(ConnectionRequestPointer iConnectionRequest);
        void _addFavourite(QVariant iRequest);
        void _removeFavourite(QVariant iRequest);

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);

        // External signal slots
    private slots:
        void _addHistory(QVariant iRequest);

    private:
        // Member data
        CachedAPI* mAPI;
        MainView* mView;
        QList<QVariant> mHistory, mFavourites;

        // Screens
        LiveboardController* mScreenLiveboard;
        RequestController* mScreenRequest;
    };
}

#endif // MAINCONTROLLER_H
