//
// Configuration
//

// Includes
#include "liveboardcontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardController::LiveboardController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = new LiveboardView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(downloadLiveboard(QString)), this, SLOT(_downloadLiveboard(QString)));
    connect(mView, SIGNAL(downloadVehicle(QString)), this, SLOT(_downloadVehicle(QString)));
    connect(mView, SIGNAL(downloadLiveboard(QString)), this, SLOT(_downloadLiveboard(QString)));
}

LiveboardController::~LiveboardController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete mView;
}

void LiveboardController::showView()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView->show();
    mView->load();
}


//
// View slots
//

void LiveboardController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    mAPI->requestStations();
}

void LiveboardController::_downloadVehicle(QString iVehicleId)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*, QDateTime)), this, SLOT(gotVehicle(VehiclePointer*, QDateTime)));
    mAPI->requestVehicle(iVehicleId);
}

void LiveboardController::_downloadLiveboard(QString iStationId)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*, QDateTime)), this, SLOT(gotLiveboard(LiveboardPointer*, QDateTime)));
    mAPI->requestLiveboard(iStationId);
}


//
// Internal slots
//

void LiveboardController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void LiveboardController::gotVehicle(VehiclePointer* iVehicle, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*, QDateTime)), this, SLOT(gotVehicle(VehiclePointer*, QDateTime)));
    if (iVehicle != 0)
        mView->setVehicle(iVehicle);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}

void LiveboardController::gotLiveboard(LiveboardPointer* iLiveboard, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*, QDateTime)), this, SLOT(gotLiveboard(LiveboardPointer*, QDateTime)));
    if (iLiveboard != 0)
        mView->setLiveboard(iLiveboard);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
