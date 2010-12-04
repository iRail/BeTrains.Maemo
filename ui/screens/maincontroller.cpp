//
// Configuration
//

// Includes
#include "maincontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainController::MainController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = new MainView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(launchLiveboard()), this, SLOT(_launchLiveboard()));
    connect(mView, SIGNAL(launchLiveboard(LiveboardRequestPointer)), this, SLOT(_launchLiveboard(LiveboardRequestPointer)));
    connect(mView, SIGNAL(launchRequest()), this, SLOT(_launchRequest()));
    connect(mView, SIGNAL(launchRequest(ConnectionRequestPointer)), this, SLOT(_launchRequest(ConnectionRequestPointer)));

    mScreenLiveboard = 0;
    mScreenRequest = 0;
}

MainController::~MainController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void MainController::showView()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->load();
    mView->show();
}


//
// View slots
//

void MainController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));

    bool tCached;
    mAPI->requestStations(tCached);
    if (!tCached)
        mView->showProgress();
}

void MainController::_launchLiveboard()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenLiveboard == 0)
    {
        mScreenLiveboard = new LiveboardController(mAPI, mView);
        connect(mScreenLiveboard, SIGNAL(addHistory(LiveboardRequestPointer)), this, SLOT(_addHistory(LiveboardRequestPointer)));
    }

    mScreenLiveboard->showView();
}

void MainController::_launchLiveboard(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenLiveboard == 0)
    {
        mScreenLiveboard = new LiveboardController(mAPI, mView);
        connect(mScreenLiveboard, SIGNAL(addHistory(LiveboardRequestPointer)), this, SLOT(_addHistory(LiveboardRequestPointer)));
    }

    mScreenLiveboard->showView(iLiveboardRequest);
}

void MainController::_launchRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenRequest == 0)
    {
        mScreenRequest = new RequestController(mAPI, mView);
        connect(mScreenRequest, SIGNAL(addHistory(ConnectionRequestPointer)), this, SLOT(_addHistory(ConnectionRequestPointer)));
    }

    mScreenRequest->showView();
}

void MainController::_launchRequest(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenRequest == 0)
    {
        mScreenRequest = new RequestController(mAPI, mView);
        connect(mScreenRequest, SIGNAL(addHistory(ConnectionRequestPointer)), this, SLOT(_addHistory(ConnectionRequestPointer)));
    }

    mScreenRequest->showView(iConnectionRequest);
}



//
// Internal slots
//

void MainController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}


//
// External signal slots
//

void MainController::_addHistory(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QVariant tRequest;
    tRequest.setValue(iLiveboardRequest);
    mHistory.push_front(tRequest);

    mView->load(mHistory);
}

void MainController::_addHistory(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QVariant tRequest;
    tRequest.setValue(iConnectionRequest);
    mHistory.push_front(tRequest);

    mView->load(mHistory);
}
