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
    connect(mView, SIGNAL(downloadVehicle(QString)), this, SLOT(_downloadVehicle(QString)));
    connect(mView, SIGNAL(downloadLiveboard(QString)), this, SLOT(_downloadLiveboard(QString)));
}

MainController::~MainController()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

    delete mView;
}

void MainController::showView()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView->show();
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

void MainController::_downloadVehicle(QString iVehicleId)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    mAPI->requestVehicle(iVehicleId);
}

void MainController::_downloadLiveboard(QString iStationId)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*)), this, SLOT(gotLiveboard(LiveboardPointer*)));
    mAPI->requestLiveboard(iStationId);
}


//
// Internal slots
//

void MainController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mView->setStations(iStations);
}

void MainController::gotConnection(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    mView->setConnections(iConnections);
}

void MainController::gotVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    mView->setVehicle(iVehicle);
}

void MainController::gotLiveboard(LiveboardPointer* iLiveboard)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*)), this, SLOT(gotLiveboard(LiveboardPointer*)));
    mView->setLiveboard(iLiveboard);
}
