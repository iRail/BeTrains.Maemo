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
    connect(mView, SIGNAL(downloadVehicles(QList<QString>)), this, SLOT(_downloadVehicles(QList<QString>)));
}

VehicleController::~VehicleController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void VehicleController::showView(ConnectionPointer iConnection)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load(iConnection);
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

void VehicleController::_downloadVehicles(QList<QString> iVehicleIds)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tVehicleIds = iVehicleIds;
    tVehicles = new QMap<QString, VehiclePointer>();
    connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
    if (iVehicleIds.count() > 0)
        mAPI->requestVehicle(iVehicleIds.at(0));
    else
        mView->setVehicles(new QMap<QString, VehiclePointer>());
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

    if (iVehicle != 0)
    {
        tVehicles->insert((*iVehicle)->id(), *iVehicle);
        if (tVehicleIds.count() > tVehicles->count())
            mAPI->requestVehicle(tVehicleIds.at(tVehicles->size()));
        else
        {
            disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
            mView->setVehicles(tVehicles);
        }
    }
    else
    {
        disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(gotVehicle(VehiclePointer*)));
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
    }


}
