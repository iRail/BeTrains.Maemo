//
// Configuration
//

// Includes
#include "vehiclecontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

VehicleController::VehicleController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = new VehicleView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(downloadVehicle(QString)), this, SLOT(_downloadVehicle(QString)));
}

VehicleController::~VehicleController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void VehicleController::showView(Connection::Line iConnectionLine)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load(iConnectionLine);
}


//
// View slots
//

void VehicleController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void VehicleController::_downloadVehicle(QString iVehicleId)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    mAPI->requestVehicle(iVehicleId);
}


//
// Internal slots
//

void VehicleController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void VehicleController::gotVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    if (iVehicle != 0)
        mView->setVehicle(iVehicle);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
