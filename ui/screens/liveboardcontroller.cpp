//
// Configuration
//

// Includes
#include "liveboardcontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardController::LiveboardController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = new LiveboardView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(downloadLiveboard(LiveboardRequestPointer)), this, SLOT(_downloadLiveboard(LiveboardRequestPointer)));
    connect(mView, SIGNAL(launchVehicle(QString,Liveboard::Departure)), this, SLOT(_launchVehicle(QString,Liveboard::Departure)));

    mScreenVehicle = 0;
}

LiveboardController::~LiveboardController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void LiveboardController::showView()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load();
}

void LiveboardController::showView(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load();  // This because the liveboard screen performs two tasks:
                    // forming the request and fetching its data. This means
                    // that using the secondary load bypasses the first
                    // stage, hence we call it manually here.
    mView->load(iLiveboardRequest);
}


//
// View slots
//

void LiveboardController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));

    bool tCached;
    mAPI->requestStations(tCached);
    if (!tCached)
        mView->showProgress();
}

void LiveboardController::_downloadLiveboard(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Manage history (but skip timed requests, from the "More" button)
    if (! iLiveboardRequest->timed())
        emit addHistory(iLiveboardRequest);

    connect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*, QDateTime)), this, SLOT(gotLiveboard(LiveboardPointer*, QDateTime)));

    bool tCached;
    mAPI->requestLiveboard(iLiveboardRequest, tCached);
    if (!tCached)
        mView->showProgress();
}

void LiveboardController::_launchVehicle(QString iStationId, Liveboard::Departure iLiveboardDeparture)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenVehicle == 0)
    {
        mScreenVehicle = new VehicleController(mAPI, mView);
    }

    Connection::Line tLine;
    tLine.departure.station = iStationId;
    tLine.departure.platform = iLiveboardDeparture.platform;
    tLine.departure.delay = iLiveboardDeparture.delay;
    tLine.departure.datetime = iLiveboardDeparture.datetime;
    tLine.arrival.station = iLiveboardDeparture.station;
    tLine.arrival.platform = 0;
    tLine.arrival.delay = 0;
    tLine.vehicle = iLiveboardDeparture.vehicle;

    mScreenVehicle->showView(tLine);
}


//
// Internal slots
//

void LiveboardController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void LiveboardController::gotLiveboard(LiveboardPointer* iLiveboard, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*, QDateTime)), this, SLOT(gotLiveboard(LiveboardPointer*, QDateTime)));
    if (iLiveboard != 0)
        mView->setLiveboard(iLiveboard);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
