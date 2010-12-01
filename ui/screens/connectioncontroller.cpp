//
// Configuration
//

// Includes
#include "connectioncontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionController::ConnectionController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = new ConnectionView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(downloadConnections(ConnectionRequestPointer)), this, SLOT(_downloadConnections(ConnectionRequestPointer)));
    connect(mView, SIGNAL(launchVehicle(Connection::Line)), this, SLOT(_launchVehicle(Connection::Line)));

    mScreenVehicle = 0;
}

ConnectionController::~ConnectionController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void ConnectionController::showView(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load(iConnectionRequest);
}


//
// View slots
//

void ConnectionController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));

    bool tCached;
    mAPI->requestStations(tCached);
    if (!tCached)
        mView->showProgress();
}

void ConnectionController::_downloadConnections(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*, QDateTime)), this, SLOT(gotConnections(QList<ConnectionPointer>*, QDateTime)));

    bool tCached;
    mAPI->requestConnections(iConnectionRequest, tCached);
    if (!tCached)
        mView->showProgress();
}

void ConnectionController::_launchVehicle(Connection::Line iConnectionLine)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenVehicle == 0)
    {
        mScreenVehicle = new VehicleController(mAPI, mView);
    }

    mScreenVehicle->showView(iConnectionLine);
}


//
// Internal slots
//

void ConnectionController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void ConnectionController::gotConnections(QList<ConnectionPointer>* iConnections, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*, QDateTime)), this, SLOT(gotConnections(QList<ConnectionPointer>*, QDateTime)));
    if (iConnections != 0)
        mView->setConnections(iConnections);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
