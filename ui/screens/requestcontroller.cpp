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

RequestController::RequestController(CachedAPI* iAPI, QWidget* iParent) : GenericController(iAPI, iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    setView(new RequestView());
    connect(view(), SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(view(), SIGNAL(launchConnection(ConnectionRequestPointer)), this, SLOT(_launchConnection(ConnectionRequestPointer)));

    mScreenConnection = 0;
}

RequestController::~RequestController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete view();
}

void RequestController::showView(GenericController* parent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    GenericController::showView(parent);
    dynamic_cast<RequestView*>(view())->load();
}

// TODO: this method just forwards the data to the following
//       widget. This won't be neccesary when we have access
//       to all screens from the application controller
//       (using a state machine).
void RequestController::showView(GenericController* parent, ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    GenericController::showView(parent);
    dynamic_cast<RequestView*>(view())->load();
    dynamic_cast<RequestView*>(view())->configure(iInitialRequest);
    emit _launchConnection(iInitialRequest);
}


//
// View slots
//

void RequestController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(api(), SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));

    bool tCached;
    api()->requestStations(tCached);
    if (!tCached)
        view()->showProgress();
}

void RequestController::_launchConnection(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    emit addHistory(QVariant::fromValue(iConnectionRequest));

    if (mScreenConnection == 0)
    {
        mScreenConnection = new ConnectionController(api(), view());
    }

    mScreenConnection->showView(iConnectionRequest);
}


//
// Internal slots
//

void RequestController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(api(), SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        dynamic_cast<RequestView*>(view())->setStations(iStations);
    else
        view()->showError( api()->hasError() ? api()->errorString() : tr("unknown error") );
}
