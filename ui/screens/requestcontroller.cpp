//
// Configuration
//

// Includes
#include "requestcontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

RequestController::RequestController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = new RequestView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(launchConnection(ConnectionRequestPointer)), this, SLOT(_launchConnection(ConnectionRequestPointer)));

    mScreenConnection = 0;
}

RequestController::~RequestController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void RequestController::showView()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->showUI();
}

void RequestController::showView(ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->showUI(iInitialRequest);
}


//
// View slots
//

void RequestController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void RequestController::_launchConnection(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenConnection == 0)
    {
        mScreenConnection = new ConnectionController(mAPI, mView);
    }

    mScreenConnection->showView(iConnectionRequest);
}


//
// Internal slots
//

void RequestController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
