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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView = new ConnectionView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(downloadConnections(ConnectionRequestPointer)), this, SLOT(_downloadConnections(ConnectionRequestPointer)));
    connect(mView, SIGNAL(launchVehicle(ConnectionPointer)), this, SLOT(_launchVehicle(ConnectionPointer)));

    mScreenVehicle = 0;
}

ConnectionController::~ConnectionController()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

    delete mView;
}

void ConnectionController::showView(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView->showUI(iConnectionRequest);
}


//
// View slots
//

void ConnectionController::_downloadStations()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void ConnectionController::_downloadConnections(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}

void ConnectionController::_launchVehicle(ConnectionPointer iConnection)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void ConnectionController::gotConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    if (iConnections != 0)
        mView->setConnections(iConnections);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
