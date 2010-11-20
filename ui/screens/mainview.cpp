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
    init_ui();
    init_children();

    mChildConnectionRequest = 0;
    mChildConnectionResult = 0;
    mChildConnectionDetail = 0;
    mChildLiveboard = 0;
}

MainView::~MainView()
{

}


//
// Initialization
//

void MainView::init_ui()
{
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
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SLOT(_showLiveboard()));
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
    mAction = CONNECTIONREQUEST;
    emit downloadStations();
}

void MainView::_showConnectionRequest(const QMap<QString, StationPointer>& iStations)
{
    if (mChildConnectionRequest == 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(iStations, this);
        mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SLOT(process_connectionrequestwidget(ConnectionRequestPointer)));
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
    mAction = CONNECTIONRESULT;
    emit downloadConnections(iConnectionRequest);
}

void MainView::_showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections)
{
    if (mChildConnectionResult == 0)
    {
        // Connection request widget
        Q_ASSERT(mChildConnectionRequest != 0);
        mChildConnectionResult = new ConnectionResultWidget(iStations, mChildConnectionRequest);
        mChildConnectionResult->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionResult->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SLOT(process_connectionresultwidget(ConnectionPointer)));
    }

    // Show the results
    mChildConnectionResult->show();
    mChildConnectionResult->load(iConnections);
}

void MainView::_showConnectionDetail(ConnectionPointer iConnection)
{
    mAction = CONNECTIONDETAIL;
    tConnection = iConnection;
    tVehicles = new QMap<QString, VehiclePointer>();
    emit downloadVehicle(iConnection->lines().at(0).vehicle);
}

void MainView::_showConnectionDetail(const QMap<QString, StationPointer>& iStations, ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
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
    mAction = LIVEBOARDREQUEST;
    emit downloadStations();
}

void MainView::_showLiveboardRequest(const QMap<QString, StationPointer>& iStations)
{
    if (mChildLiveboard == 0)
    {
        // Connection request widget
        mChildLiveboard = new LiveboardWidget(iStations, this);
        mChildLiveboard->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildLiveboard->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildLiveboard, SIGNAL(finished(Liveboard::Departure)), this, SLOT(process_liveboardwidget(Liveboard::Departure)));
        connect(mChildLiveboard, SIGNAL(request(QString)), this, SLOT(process_liveboardwidget_station(QString)));
    }

    mChildLiveboard->clear();
    mChildLiveboard->show();
}

void MainView::_showLiveboardResult(StationPointer iStation)
{
    mAction = LIVEBOARDRESULT;
    emit downloadLiveboard(iStation->id());
}

void MainView::_showLiveboardResult(const QMap<QString, StationPointer>& iStations, LiveboardPointer iLiveboard)
{
    Q_ASSERT(mChildLiveboard != 0);

    // Connection request widget
    mChildLiveboard->load(iLiveboard);
}

void MainView::load_history(QModelIndex iIndex)
{
    tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    _showConnectionRequest();
}


//
// Controller actions
//

void MainView::setStations(QMap<QString, StationPointer>* iStations)
{
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
    switch (mAction)
    {
    case CONNECTIONRESULT:
        tConnections = iConnections;
        emit downloadStations();
        break;
    default:
        // TODO: warn
        break;
    }
}

void MainView::setVehicle(VehiclePointer* iVehicle)
{
    switch (mAction)
    {
    case CONNECTIONDETAIL:
        tVehicles->insert((*iVehicle)->id(), *iVehicle);
        emit downloadVehicle(tConnection->lines().at(tVehicles->size()).vehicle);
        break;
    default:
        // TODO: warn
        break;
    }
}

void MainView::setLiveboard(LiveboardPointer* iLiveboard)
{
    switch (mAction)
    {
    case LIVEBOARDRESULT:
        tLiveboard = *iLiveboard;
        emit downloadStations();
        break;
    default:
        // TODO: warn
        break;
    }
}


//
// Auxiliary
//

void MainView::populateModel()
{
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
