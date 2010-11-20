//
// Configuration
//

// Includes
#include "mainview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainView::MainView(QWidget* iParent) : QScrollArea(iParent)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    init_ui();
    init_children();

    mChildConnectionRequest = 0;
    mChildConnectionResult = 0;
    mChildConnectionDetail = 0;
    mChildLiveboard = 0;
}

MainView::~MainView()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}


//
// Initialization
//

void MainView::init_ui()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Window settings
    this->setWindowTitle(QString("BeTrains"));
    this->setAttribute(Qt::WA_Maemo5StackedWindow);

    // Parent widget
    QWidget *tWidget = new QWidget();
    setWidget(tWidget);
    setWidgetResizable(true);

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    tWidget->setLayout(layout);

    // Top buttons
    QHBoxLayout *blayout = new QHBoxLayout;
    mUIButtonSearch = new QPushButton(tr("Plan a journey"));
    mUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    blayout->addWidget(mUIButtonSearch);
    blayout->addWidget(mUIButtonLiveboard);
    connect(mUIButtonSearch, SIGNAL(clicked()), this, SLOT(_showConnectionRequest()));
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SLOT(_showLiveboardRequest()));
    layout->addLayout(blayout);

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setItemDelegate(new ConnectionRequestDelegate());
    mView->setResizeMode(QListView::Adjust);
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(load_history(QModelIndex)));
    layout->addWidget(mView);
    // TODO: configure the QListView to be expanding within the QScrollArea

    // Create the history listview dummy
    mViewDummy = new QLabel(tr("No history or favorites"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    layout->addWidget(mViewDummy);
    // TODO: load history from file
    populateModel();

}

void MainView::init_children()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    //connect(mAPI, SIGNAL(miss()), mChildProgressDialog, SLOT(show()));
    //connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}


//
// UI events
//

void MainView::_showConnectionRequest()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = CONNECTIONREQUEST;
    emit downloadStations();
}

void MainView::_showConnectionRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionRequest == 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(iStations, this);
        mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SLOT(_showConnectionResult(ConnectionRequestPointer)));
    }

    mChildConnectionRequest->clear();
    if (!tInitialRequest.isNull())
    {
        mChildConnectionRequest->load(tInitialRequest);
        tInitialRequest.clear();
    }
    mChildConnectionRequest->show();
}

void MainView::_showConnectionResult(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = CONNECTIONRESULT;
    emit downloadConnections(iConnectionRequest);
}

void MainView::_showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionResult == 0)
    {
        // Connection request widget
        Q_ASSERT(mChildConnectionRequest != 0);
        mChildConnectionResult = new ConnectionResultWidget(iStations, mChildConnectionRequest);
        mChildConnectionResult->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionResult->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SLOT(_showConnectionDetail(ConnectionPointer)));
    }

    // Show the results
    mChildConnectionResult->show();
    mChildConnectionResult->load(iConnections);
}

void MainView::_showConnectionDetail(ConnectionPointer iConnection)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = CONNECTIONDETAIL;
    tConnection = iConnection;
    tVehicles = new QMap<QString, VehiclePointer>();
    emit downloadVehicle(iConnection->lines().at(0).vehicle);
}

void MainView::_showConnectionDetail(const QMap<QString, StationPointer>& iStations, ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionDetail == 0)
    {
        // Connection request widget
        Q_ASSERT(mChildConnectionResult != 0);
        mChildConnectionDetail = new ConnectionDetailWidget(iStations, mChildConnectionResult);
        mChildConnectionDetail->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionDetail->setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    // Show the results
    mChildConnectionDetail->show();
    mChildConnectionDetail->load(iConnection, iVehicles);
}

void MainView::_showLiveboardRequest()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = LIVEBOARDREQUEST;
    emit downloadStations();
}

void MainView::_showLiveboardRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildLiveboard == 0)
    {
        // Connection request widget
        mChildLiveboard = new LiveboardWidget(iStations, this);
        mChildLiveboard->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildLiveboard->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildLiveboard, SIGNAL(request(QString)), this, SLOT(_showLiveboardResult(QString)));
        //connect(mChildLiveboard, SIGNAL(finished(Liveboard::Departure)), this, SLOT(process_liveboardwidget(Liveboard::Departure)));
    }

    mChildLiveboard->clear();
    mChildLiveboard->show();
}

void MainView::_showLiveboardResult(QString iStationId)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = LIVEBOARDRESULT;
    emit downloadLiveboard(iStationId);
}

void MainView::_showLiveboardResult(const QMap<QString, StationPointer>& iStations, LiveboardPointer iLiveboard)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;
    Q_UNUSED(iStations);
    Q_ASSERT(mChildLiveboard != 0);

    // Connection request widget
    mChildLiveboard->load(iLiveboard);
}

void MainView::load_history(QModelIndex iIndex)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    _showConnectionRequest();
}


//
// Controller actions
//

void MainView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    switch (mAction)
    {
    case CONNECTIONREQUEST:
        _showConnectionRequest(*iStations);
        delete iStations;
        break;
    case CONNECTIONRESULT:
        _showConnectionResult(*iStations, *tConnections);
        delete iStations;
        delete tConnections;
        break;
    case CONNECTIONDETAIL:
        _showConnectionDetail(*iStations, tConnection, *tVehicles);
        delete iStations;
        delete tVehicles;
        tConnection.clear();
        break;
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

void MainView::setConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    switch (mAction)
    {
    case CONNECTIONRESULT:
        tConnections = iConnections;
        emit downloadStations();
        break;
    default:
        qWarning() << "! " << "Action" << mAction << "isn't implemented here!";
        break;
    }
}

void MainView::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    switch (mAction)
    {
    case CONNECTIONDETAIL:
        tVehicles->insert((*iVehicle)->id(), *iVehicle);
        if (tConnection->lines().count() > tVehicles->count())
            emit downloadVehicle(tConnection->lines().at(tVehicles->size()).vehicle);
        else
            emit downloadStations();
        break;
    default:
        qWarning() << "! " << "Action" << mAction << "isn't implemented here!";
        break;
    }
}

void MainView::setLiveboard(LiveboardPointer* iLiveboard)
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

void MainView::showError(const QString &iError)
{
    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}


//
// Auxiliary
//

void MainView::populateModel()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mModel->clear();
    if (mConnectionRequestHistory.size() > 0)
    {
        for (int i = 0; i < mConnectionRequestHistory.size(); i++)
        {
            ConnectionRequestPointer tConnectionRequest = mConnectionRequestHistory.at(i);
            QStandardItem *tItem = new QStandardItem;

            tItem->setData(QVariant::fromValue(tConnectionRequest), ConnectionRequestRole);
            mModel->appendRow(tItem);
        }

        mViewDummy->setVisible(false);
        mView->setVisible(true);
        mView->setModel(mModel);
        mView->setFixedHeight(70*mModel->rowCount());   // HACK
    }
    else
    {
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
}
