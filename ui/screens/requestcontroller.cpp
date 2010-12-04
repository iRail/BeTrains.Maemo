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

    mView->show();
    mView->load();
}

// TODO: this method just forwards the data to the following
//       widget. This won't be neccesary when we have access
//       to all screens from the application controller
//       (using a state machine).
void RequestController::showView(ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load();
    mView->configure(iInitialRequest);
    emit _launchConnection(iInitialRequest);
}


//
// View slots
//

void RequestController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));

    bool tCached;
    mAPI->requestStations(tCached);
    if (!tCached)
        mView->showProgress();
}

void RequestController::_launchConnection(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    emit addHistory(iConnectionRequest);

    if (mScreenConnection == 0)
    {
        mScreenConnection = new ConnectionController(mAPI, mView);
    }

    mScreenConnection->showView(iConnectionRequest);
}


//
// Internal slots
//

void RequestController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
