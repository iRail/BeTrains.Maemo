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
    connect(mView, SIGNAL(launchVehicle(ConnectionPointer)), this, SLOT(_launchVehicle(ConnectionPointer)));

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

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void ConnectionController::_downloadConnections(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}

void ConnectionController::_launchVehicle(ConnectionPointer iConnection)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenVehicle == 0)
    {
        mScreenVehicle = new VehicleController(mAPI, mView);
    }

    mScreenVehicle->showView(iConnection);
}


//
// Internal slots
//

void ConnectionController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void ConnectionController::gotConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    if (iConnections != 0)
        mView->setConnections(iConnections);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
