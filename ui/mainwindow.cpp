//
// Configuration
//

// Includes
#include "mainwindow.h"
#include "auxiliary/delegates/connectionrequestdelegate.h"
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

MainWindow::MainWindow(CachedAPI* iAPI, QWidget* parent) : QScrollArea(parent), mAPI(iAPI)
{
    init_ui();
    init_children();

    mChildConnectionRequest = 0;
    mChildConnectionResult = 0;
    mChildConnectionDetail = 0;
    mChildLiveboard = 0;
}

MainWindow::~MainWindow()
{
    delete mChildProgressDialog;
}

//
// Initialization
//

void MainWindow::init_ui()
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
    connect(mUIButtonSearch, SIGNAL(clicked()), this, SLOT(show_requestwidget()));
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SLOT(show_liveboardwidget()));
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

void MainWindow::init_children()
{    
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(miss()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}


//
// Public slots
//


void MainWindow::load_requestwidget(QMap<QString, StationPointer>* iStations)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_requestwidget(QMap<QString, StationPointer>*)));

    if (iStations != 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(*iStations, this);
        mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SLOT(process_requestwidget(ConnectionRequestPointer)));

        // Finish up
        delete iStations;

        // Actually show the widget
        show_requestwidget();
    }
    else
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }
}

void MainWindow::show_requestwidget()
{
    // Check if the widget is loaded already
    if (mChildConnectionRequest == 0)
    {
        mChildProgressDialog->setEnabled(true);
        mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));
        connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_requestwidget(QMap<QString, StationPointer>*)));
        mAPI->requestStations();
        return;
    }

    mChildConnectionRequest->clear();
    if (!tInitialRequest.isNull())
    {
        mChildConnectionRequest->load(tInitialRequest);
        tInitialRequest.clear();
    }
    mChildConnectionRequest->show();
}

void MainWindow::process_requestwidget(ConnectionRequestPointer iConnectionRequest)
{
    // Fix the history model
    mConnectionRequestHistory.prepend(iConnectionRequest);
    populateModel();

    // Request the data
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of connections"));
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_resultwidget(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}

void MainWindow::load_resultwidget()
{
    // Result widget
    Q_ASSERT(mChildConnectionRequest != 0);
    mChildConnectionResult = new ConnectionResultWidget(mChildConnectionRequest->stations(), mChildConnectionRequest);
    mChildConnectionResult->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionResult->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SLOT(process_resultwidget(ConnectionPointer)));
}

void MainWindow::show_resultwidget(QList<ConnectionPointer>* iConnections)
{
    // Disconnect the signal (as the only way this widget can be shown
    // is through the API)
    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_resultwidget(QList<ConnectionPointer>*)));
    mChildProgressDialog->setEnabled(false);

    // Check if the widget is loaded already
    if (mChildConnectionResult == 0)
        load_resultwidget();


    if (iConnections != 0)
    {
        // Show the results
        mChildConnectionResult->show();
        mChildConnectionResult->load(*iConnections);
        delete iConnections;
    }
    else
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);

    }
}

void MainWindow::process_resultwidget(ConnectionPointer iConnection)
{
    tConnection = iConnection;
    tVehicles.clear();
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching vehicle information"));
    show_detailwidget(iConnection);
}

void MainWindow::load_detailwidget_vehicle(VehiclePointer* iVehicle)
{
    disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(load_detailwidget_vehicle(VehiclePointer*)));
    tVehicles.insert((*iVehicle)->id(), *iVehicle);
    show_detailwidget(tConnection);
}

void MainWindow::load_detailwidget()
{
    // Detail widget
    Q_ASSERT(mChildConnectionResult != 0);
    mChildConnectionDetail = new ConnectionDetailWidget(mChildConnectionRequest->stations(), mChildConnectionResult);
    mChildConnectionDetail->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionDetail->setAttribute(Qt::WA_Maemo5StackedWindow);
}

void MainWindow::show_detailwidget(ConnectionPointer iConnection)
{
    // Check if the widget is loaded already
    if (mChildConnectionDetail == 0)
        load_detailwidget();

    // Check if we got all the vehicle data
    for (int i = 0; i < tConnection->lines().size(); i++)
    {
        QString tVehicleId = tConnection->lines().at(i).vehicle;
        if (! tVehicles.contains(tVehicleId))
        {
            connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(load_detailwidget_vehicle(VehiclePointer*)));
            mAPI->requestVehicle(tVehicleId);
            return;
        }
    }
    mChildProgressDialog->setEnabled(false);

    // Show the details
    mChildConnectionDetail->show();
    mChildConnectionDetail->load(iConnection, tVehicles);
}
void MainWindow::load_liveboardwidget(QMap<QString, StationPointer>* iStations)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_liveboardwidget(QMap<QString, StationPointer>*)));

    if (iStations != 0)
    {
        // Connection request widget
        mChildLiveboard = new LiveboardWidget(*iStations, this);
        mChildLiveboard->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildLiveboard->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildLiveboard, SIGNAL(finished(Liveboard::Departure)), this, SLOT(process_liveboardwidget(Liveboard::Departure)));

        // Finish up
        delete iStations;

        // Actually show the widget
        show_liveboardwidget();
    }
    else
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }
}

void MainWindow::show_liveboardwidget()
{
    // Check if the widget is loaded already
    if (mChildLiveboard == 0)
    {
        mChildProgressDialog->setEnabled(true);
        mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));
        connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_liveboardwidget(QMap<QString, StationPointer>*)));
        mAPI->requestStations();
        return;
    }

    mChildLiveboard->clear();
    mChildLiveboard->show();
}

void MainWindow::process_liveboardwidget(Liveboard::Departure iDeparture)
{

}

//
// UI events
//

void MainWindow::load_history(QModelIndex iIndex)
{
    // Bug in Qt? Non-selectable QStandardItem can be doubleClicked...
    if (mConnectionRequestHistory.size() == 0)
        return;

    tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    show_requestwidget();
}


//
// Auxiliary
//

void MainWindow::populateModel()
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
