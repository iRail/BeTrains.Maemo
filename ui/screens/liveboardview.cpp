//
// Configuration
//

// Includes
#include "liveboardview.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardView::LiveboardView(QWidget* iParent) : GenericView(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);

    mChildLiveboard = 0;
}

LiveboardView::~LiveboardView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}

void LiveboardView::showUI()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showLiveboardRequest();
    GenericView::showUI();
}


//
// UI events
//

void LiveboardView::_showLiveboardRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mAction = LIVEBOARDREQUEST;
    emit downloadStations();
}

void LiveboardView::_showLiveboardRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mChildLiveboard == 0)
    {
        // Connection request widget
        // TODO: create the UI here (no separate widgets) so we can hook in the loader
        mChildLiveboard = new LiveboardWidget(iStations, this);
        mUILayout->addWidget(mChildLiveboard);
        connect(mChildLiveboard, SIGNAL(request(QString)), this, SLOT(_showLiveboardResult(QString)));
        //connect(mChildLiveboard, SIGNAL(finished(Liveboard::Departure)), this, SLOT(process_liveboardwidget(Liveboard::Departure)));
    }

    mChildLiveboard->clear();
    mChildLiveboard->show();
}

void LiveboardView::_showLiveboardResult(QString iStationId)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mAction = LIVEBOARDRESULT;
    emit downloadLiveboard(iStationId);
}

void LiveboardView::_showLiveboardResult(const QMap<QString, StationPointer>& iStations, LiveboardPointer iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    Q_UNUSED(iStations);
    Q_ASSERT(mChildLiveboard != 0);

    // Connection request widget
    mChildLiveboard->load(iLiveboard);
}


//
// Controller actions
//

void LiveboardView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    switch (mAction)
    {
    case LIVEBOARDREQUEST:
        _showLiveboardRequest(*iStations);
        delete iStations;
        break;
    case LIVEBOARDRESULT:
        _showLiveboardResult(*iStations, tLiveboard);
        delete iStations;
        tLiveboard.clear();
        break;
    }
}

void LiveboardView::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    switch (mAction)
    {
    default:
        qWarning() << "! " << "Action" << mAction << "isn't implemented here!";
        break;
    }
}

void LiveboardView::setLiveboard(LiveboardPointer* iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    switch (mAction)
    {
    case LIVEBOARDRESULT:
        tLiveboard = *iLiveboard;
        emit downloadStations();
        break;
    default:
        qWarning() << "! " << "Action" << mAction << "isn't implemented here!";
        break;
    }
}
