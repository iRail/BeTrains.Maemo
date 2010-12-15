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
#include "genericcontroller.h"


namespace iRail
{
    class MainController : public GenericController
    {
    Q_OBJECT
    public:
        explicit MainController(CachedAPI* iAPI, QWidget *parent = 0);
        ~MainController();
        void showView(GenericController* parent = 0);

        // View slots
    private slots:
        void _downloadStations();
        void _getHistoryFavourites();
        void _launchLiveboard(LiveboardRequestPointer iLiveboardRequest);
        void _launchRequest();
        void _launchRequest(ConnectionRequestPointer iConnectionRequest);
        void _addFavourite(QVariant iRequest);
        void _removeFavourite(QVariant iRequest);
        void _clearHistory();

        // Internal slots
    private slots:
        void gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp);

        // External signal slots
    private slots:
        void _addHistory(QVariant iRequest);

        // Signals
    signals:
        void launchLiveboard();

    private:
        // Member data
        QList<QVariant> mHistory, mFavourites;

        // Screens
        LiveboardController* mScreenLiveboard;
        RequestController* mScreenRequest;
    };
}

#endif // MAINCONTROLLER_H
