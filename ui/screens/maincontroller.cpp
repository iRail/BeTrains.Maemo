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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView = new MainView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(downloadConnections(ConnectionRequestPointer)), this, SLOT(_downloadConnections(ConnectionRequestPointer)));
    connect(mView, SIGNAL(launchLiveboard()), this, SLOT(_launchLiveboard()));
    connect(mView, SIGNAL(launchVehicle(ConnectionPointer)), this, SLOT(_launchVehicle(ConnectionPointer)));

    mScreenLiveboard = 0;
    mScreenVehicle = 0;
}

MainController::~MainController()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

    delete mView;
}

void MainController::showView()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView->showUI();
}


//
// View slots
//

void MainController::_downloadStations()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void MainController::_downloadConnections(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}

void MainController::_launchLiveboard()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mScreenLiveboard == 0)
    {
        mScreenLiveboard = new LiveboardController(mAPI, mView);
    }

    mScreenLiveboard->showView();
}

void MainController::_launchVehicle(ConnectionPointer iConnection)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mScreenVehicle == 0)
    {
        mScreenVehicle = new VehicleController(mAPI, mView->mChildConnectionResult);
    }

    mScreenVehicle->showView(iConnection);
}


//
// Internal slots
//

void MainController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void MainController::gotConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    if (iConnections != 0)
        mView->setConnections(iConnections);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

