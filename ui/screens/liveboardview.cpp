//
// Configuration
//

// Includes
#include "liveboardview.h"
#include <QtMaemo5/QMaemo5InformationBox>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardView::LiveboardView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);

    mChildLiveboard = 0;

    this->hide();
}

LiveboardView::~LiveboardView()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void LiveboardView::showUI()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showLiveboardRequest();
    this->show();
}


//
// UI events
//

void LiveboardView::_showLiveboardRequest()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = LIVEBOARDREQUEST;
    emit downloadStations();
}

void LiveboardView::_showLiveboardRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildLiveboard == 0)
    {
        // Connection request widget
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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = LIVEBOARDRESULT;
    emit downloadLiveboard(iStationId);
}

void LiveboardView::_showLiveboardResult(const QMap<QString, StationPointer>& iStations, LiveboardPointer iLiveboard)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;
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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    switch (mAction)
    {
    default:
        qWarning() << "! " << "Action" << mAction << "isn't implemented here!";
        break;
    }
}

void LiveboardView::setLiveboard(LiveboardPointer* iLiveboard)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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

void LiveboardView::showError(const QString &iError)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}

