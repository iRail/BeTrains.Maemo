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

RequestController::RequestController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView = new RequestView(iParent);
    connect(mView, SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(mView, SIGNAL(launchConnection(ConnectionRequestPointer)), this, SLOT(_launchConnection(ConnectionRequestPointer)));

    mScreenConnection = 0;
}

RequestController::~RequestController()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

    delete mView;
}

void RequestController::showView()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView->showUI();
}

void RequestController::showView(ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mView->showUI(iInitialRequest);
}


//
// View slots
//

void RequestController::_downloadStations()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    mAPI->requestStations();
}

void RequestController::_launchConnection(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mScreenConnection == 0)
    {
        mScreenConnection = new ConnectionController(mAPI, mView);
    }

    mScreenConnection->showView(iConnectionRequest);
}


//
// Internal slots
//

void RequestController::gotStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(gotStations(QMap<QString, StationPointer>*)));
    if (iStations != 0)
        mView->setStations(iStations);
    else
        mView->showError( mAPI->hasError() ? mAPI->errorString() : tr("unknown error") );
}
