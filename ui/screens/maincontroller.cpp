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
    connect(mView, SIGNAL(launchRequest()), this, SLOT(_launchRequest()));

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

    mView->show();
}


//
// View slots
//

void MainController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void MainController::_launchLiveboard()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenLiveboard == 0)
    {
        mScreenLiveboard = new LiveboardController(mAPI, mView);
    }

    mScreenLiveboard->showView();
}

void MainController::_launchRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenRequest == 0)
    {
        mScreenRequest = new RequestController(mAPI, mView);
    }

    if (!mInitialRequest.isNull())
        mScreenRequest->showView(mInitialRequest);
    else
        mScreenRequest->showView();
}

void MainController::_setInitialRequest(ConnectionRequestPointer iInitialRequest)
{
    mInitialRequest = iInitialRequest;
}


//
// Internal slots
//

void MainController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
