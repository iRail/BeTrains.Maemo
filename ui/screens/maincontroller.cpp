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
    mView = new MainView(iParent);
}

MainController::~MainController()
{
    delete mView;
}

void MainController::showView()
{
    mView->show();
}


//
// View slots
//

void MainController::_downloadStations()
{
    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void MainController::_downloadConnections(ConnectionRequestPointer iConnectionRequest)
{
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}

void MainController::_downloadVehicle(QString iVehicleId)
{
    connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    mAPI->requestVehicle(iVehicleId);
}

void MainController::_downloadLiveboard(QString iStationId)
{
    connect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*)), this, SLOT(gotLiveboard(LiveboardPointer*)));
    mAPI->requestLiveboard(iStationId);
}


//
// Internal slots
//

void MainController::gotStations(QMap<QString, StationPointer>* iStations)
{
    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mView->setStations(iStations);
}

void MainController::gotConnection(QList<ConnectionPointer>* iConnections)
{
    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(gotConnections(QList<ConnectionPointer>*)));
    mView->setConnections(iConnections);
}

void MainController::gotVehicle(VehiclePointer* iVehicle)
{
    disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    mView->setVehicle(iVehicle);
}

void MainController::gotLiveboard(LiveboardPointer* iLiveboard)
{
    disconnect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*)), this, SLOT(gotLiveboard(LiveboardPointer*)));
    mView->setLiveboard(iLiveboard);
}
