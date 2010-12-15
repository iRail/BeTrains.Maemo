//
// Configuration
//

// Includes
#include "maincontroller.h"
#include "application.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainController::MainController(CachedAPI* iAPI, QWidget* iParent) : GenericController(iAPI, iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    setView(new MainView());
    connect(view(), SIGNAL(downloadStations()), this, SLOT(_downloadStations()));
    connect(view(), SIGNAL(getHistoryFavourites()), this, SLOT(_getHistoryFavourites()));
    connect(view(), SIGNAL(launchLiveboard()), this, SIGNAL(launchLiveboard()));
    connect(view(), SIGNAL(launchLiveboard(LiveboardRequestPointer)), this, SLOT(_launchLiveboard(LiveboardRequestPointer)));
    connect(view(), SIGNAL(launchRequest()), this, SLOT(_launchRequest()));
    connect(view(), SIGNAL(launchRequest(ConnectionRequestPointer)), this, SLOT(_launchRequest(ConnectionRequestPointer)));
    connect(view(), SIGNAL(addFavourite(QVariant)), this, SLOT(_addFavourite(QVariant)));
    connect(view(), SIGNAL(removeFavourite(QVariant)), this, SLOT(_removeFavourite(QVariant)));
    connect(view(), SIGNAL(clearHistory()), this, SLOT(_clearHistory()));

    mScreenLiveboard = 0;
    mScreenRequest = 0;
}

MainController::~MainController()
{
    qDebug() << "~ " << Q_FUNC_INFO;

    delete view();
}

void MainController::showView(GenericController* parent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // FIXME: switching order wreaks havok??
    dynamic_cast<MainView*>(view())->load();
    GenericController::showView(parent);
}


//
// View slots
//

void MainController::_downloadStations()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    connect(api(), SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));

    bool tCached;
    api()->requestStations(tCached);
    if (!tCached)
        view()->showProgress();
}

void MainController::_getHistoryFavourites()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mHistory = Application::storage()->history();
    mFavourites = Application::storage()->favourites();

    dynamic_cast<MainView*>(view())->load(mHistory, mFavourites);

}

void MainController::_launchLiveboard(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenLiveboard == 0)
    {
        mScreenLiveboard = new LiveboardController(api(), view());
        connect(mScreenLiveboard, SIGNAL(addHistory(QVariant)), this, SLOT(_addHistory(QVariant)));
    }

    //mScreenLiveboard->showView(iLiveboardRequest);
}

void MainController::_launchRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenRequest == 0)
    {
        mScreenRequest = new RequestController(api(), view());
        connect(mScreenRequest, SIGNAL(addHistory(QVariant)), this, SLOT(_addHistory(QVariant)));
    }

    mScreenRequest->showView();
}

void MainController::_launchRequest(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mScreenRequest == 0)
    {
        mScreenRequest = new RequestController(api(), view());
        connect(mScreenRequest, SIGNAL(addHistory(QVariant)), this, SLOT(_addHistory(QVariant)));
    }

    mScreenRequest->showView(iConnectionRequest);
}

void MainController::_addFavourite(QVariant iRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mFavourites.push_back(iRequest);
    mHistory.removeOne(iRequest);

    Application::storage()->setHistory(mHistory);
    Application::storage()->setFavourites(mFavourites);

    dynamic_cast<MainView*>(view())->load(mHistory, mFavourites);
}

void MainController::_removeFavourite(QVariant iRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mFavourites.removeOne(iRequest);

    Application::storage()->setFavourites(mFavourites);

    dynamic_cast<MainView*>(view())->load(mHistory, mFavourites);
}

void MainController::_clearHistory()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mHistory.clear();

    Application::storage()->clearHistory();

    dynamic_cast<MainView*>(view())->load(mHistory, mFavourites);
}



//
// Internal slots
//

void MainController::gotStations(QMap<QString, StationPointer>* iStations, QDateTime iTimestamp)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    disconnect(api(), SIGNAL(replyStations(QMap<QString, StationPointer>*, QDateTime)), this, SLOT(gotStations(QMap<QString, StationPointer>*, QDateTime)));
    if (iStations != 0)
        dynamic_cast<MainView*>(view())->setStations(iStations);
    else
        view()->showError( api()->hasError() ? api()->errorString() : tr("unknown error") );
}


//
// External signal slots
//

void MainController::_addHistory(QVariant iRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mHistory.push_front(iRequest);

    Application::storage()->setHistory(mHistory);

    dynamic_cast<MainView*>(view())->load(mHistory, mFavourites);
}
