//
// Configuration
//

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainController::MainController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    mView = new MainView(iParent);

    mChildConnectionRequest = 0;
    mChildConnectionResult = 0;
    mChildConnectionDetail = 0;
    mChildLiveboard = 0;
}

MainController::~MainController()
{
    delete mChildProgressDialog;
    delete mView;
}


//
// Public slots
//

void MainController::load_stations(QObject* iObject, const char* iSlot)
{
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));
    connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(process_stations(QMap<QString, StationPointer>*)));
    connect(this, SIGNAL(reply_stations(QMap<QString, StationPointer>*)), iObject, iSlot);
    mAPI->requestStations();
}

void MainController::process_stations(QMap<QString, StationPointer>* iStations)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_connectionrequestwidget(QMap<QString, StationPointer>*)));

    if (iStations == 0)
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }

    emit reply_stations(iStations);
    // TODO: disconnect this slot automatically, so we should only emit upon != 0
}

void MainController::disconnect_stations(QObject* iObject, const char* iSlot)
{
    disconnect(this, SIGNAL(reply_stations(QMap<QString, StationPointer>*)), iObject, iSlot);
}

void MainController::load_connections(ConnectionRequestPointer iConnectionRequest, QObject* iObject, const char* iSlot)
{
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(process_connections(QList<ConnectionPointer>*)));
    connect(this, SIGNAL(reply_connections(QList<ConnectionPointer>*)), iObject, iSlot);
    mAPI->requestConnections(iConnectionRequest);
}

void MainController::process_connections(QList<ConnectionPointer>* iConnections)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(process_connections(QList<ConnectionPointer>*)));

    if (iConnections == 0)
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }

    emit reply_connections(iConnections);
    // TODO: disconnect this slot automatically, so we should only emit upon != 0
}

void MainController::disconnect_connections(QObject* iObject, const char* iSlot)
{
    disconnect(this, SIGNAL(reply_connections(QList<ConnectionPointer>*)), iObject, iSlot);
}

void MainController::load_connectionrequestwidget(QMap<QString, StationPointer>* iStations)
{
    disconnect_stations(this, SLOT(load_connectionrequestwidget(QMap<QString, StationPointer>*)));
    if (iStations != 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(*iStations, this);
        mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SLOT(process_connectionrequestwidget(ConnectionRequestPointer)));

        // Finish up
        delete iStations;

        // Actually show the widget
        show_connectionrequestwidget();
    }
}

void MainController::show_connectionrequestwidget()
{
    // Check if the widget is loaded already
    if (mChildConnectionRequest == 0)
    {
        load_stations(this, SLOT(load_connectionrequestwidget(QMap<QString, StationPointer>*)));
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

void MainController::process_connectionrequestwidget(ConnectionRequestPointer iConnectionRequest)
{
    // Fix the history model
    mConnectionRequestHistory.prepend(iConnectionRequest);
    populateModel();

    // Request the data
    load_connections(iConnectionRequest, this, SLOT(show_connectionresultwidget(QList<ConnectionPointer>*)));
}

void MainController::load_connectionresultwidget()
{
    // Result widget
    Q_ASSERT(mChildConnectionRequest != 0);
    mChildConnectionResult = new ConnectionResultWidget(mChildConnectionRequest->stations(), mChildConnectionRequest);
    mChildConnectionResult->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionResult->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SLOT(process_connectionresultwidget(ConnectionPointer)));
}

void MainController::load_vehicle(QString iVehicleId, QObject* iObject, const char* iSlot)
{
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching vehicle information"));
    connect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(process_vehicle(VehiclePointer*)));
    connect(this, SIGNAL(reply_vehicle(VehiclePointer*)), iObject, iSlot);
    mAPI->requestVehicle(iVehicleId);
}

void MainController::process_vehicle(VehiclePointer* iVehicle)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyVehicle(VehiclePointer*)), this, SLOT(process_vehicle(VehiclePointer*)));

    if (iVehicle == 0)
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }

    emit reply_vehicle(iVehicle);
    // TODO: disconnect this slot automatically, so we should only emit upon != 0
}

void MainController::disconnect_vehicle(QObject* iObject, const char* iSlot)
{
    disconnect(this, SIGNAL(reply_vehicle(VehiclePointer*)), iObject, iSlot);
}

void MainController::load_liveboard(QString iStationId, QObject* iObject, const char* iSlot)
{
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching liveboard"));
    connect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*)), this, SLOT(process_liveboard(LiveboardPointer*)));
    connect(this, SIGNAL(reply_liveboard(LiveboardPointer*)), iObject, iSlot);
    mAPI->requestLiveboard(iStationId);
}

void MainController::process_liveboard(LiveboardPointer* iLiveboard)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyLiveboard(LiveboardPointer*)), this, SLOT(process_liveboard(LiveboardPointer*)));

    if (iLiveboard == 0)
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }

    emit reply_liveboard(iLiveboard);
    // TODO: disconnect this slot automatically, so we should only emit upon != 0
}

void MainController::disconnect_liveboard(QObject* iObject, const char* iSlot)
{
    disconnect(this, SIGNAL(reply_liveboard(LiveboardPointer*)), iObject, iSlot);
}

void MainController::show_connectionresultwidget(QList<ConnectionPointer>* iConnections)
{
    disconnect_connections(this, SLOT(show_connectionresultwidget(QList<ConnectionPointer>*)));

    // Check if the widget is loaded already
    if (mChildConnectionResult == 0)
        load_connectionresultwidget();

    if (iConnections != 0)
    {
        // Show the results
        mChildConnectionResult->show();
        mChildConnectionResult->load(*iConnections);
        delete iConnections;
    }
}

void MainController::process_connectionresultwidget(ConnectionPointer iConnection)
{
    tConnection = iConnection;
    tVehicles.clear();
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching vehicle information"));
    show_connectiondetailwidget(iConnection);
}

void MainController::load_connectiondetailwidget_vehicle(VehiclePointer* iVehicle)
{
    disconnect_vehicle(this, SLOT(load_connectiondetailwidget_vehicle(VehiclePointer*)));
    tVehicles.insert((*iVehicle)->id(), *iVehicle);
    show_connectiondetailwidget(tConnection);
}

void MainController::load_connectiondetailwidget()
{
    // Detail widget
    Q_ASSERT(mChildConnectionResult != 0);
    mChildConnectionDetail = new ConnectionDetailWidget(mChildConnectionRequest->stations(), mChildConnectionResult);
    mChildConnectionDetail->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionDetail->setAttribute(Qt::WA_Maemo5StackedWindow);
}

void MainController::show_connectiondetailwidget(ConnectionPointer iConnection)
{
    // Check if the widget is loaded already
    if (mChildConnectionDetail == 0)
        load_connectiondetailwidget();

    // Check if we got all the vehicle data
    for (int i = 0; i < tConnection->lines().size(); i++)
    {
        QString tVehicleId = tConnection->lines().at(i).vehicle;
        if (! tVehicles.contains(tVehicleId))
        {
            load_vehicle(tVehicleId, this, SLOT(load_connectiondetailwidget_vehicle(VehiclePointer*)));
            return;
        }
    }
    mChildProgressDialog->setEnabled(false);

    // Show the details
    mChildConnectionDetail->show();
    mChildConnectionDetail->load(iConnection, tVehicles);
}

void MainController::load_liveboardwidget(QMap<QString, StationPointer>* iStations)
{
    disconnect_stations(this, SLOT(load_liveboardwidget(QMap<QString, StationPointer>*)));
    if (iStations != 0)
    {
        // Connection request widget
        mChildLiveboard = new LiveboardWidget(*iStations, this);
        mChildLiveboard->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildLiveboard->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildLiveboard, SIGNAL(finished(Liveboard::Departure)), this, SLOT(process_liveboardwidget(Liveboard::Departure)));
        connect(mChildLiveboard, SIGNAL(request(QString)), this, SLOT(process_liveboardwidget_station(QString)));

        // Finish up
        delete iStations;

        // Actually show the widget
        show_liveboardwidget();
    }
}

void MainController::load_liveboardwidget_liveboard(LiveboardPointer* iLiveboard)
{
    disconnect_liveboard(this, SLOT(load_liveboardwidget_liveboard(LiveboardPointer*)));

    if (iLiveboard != 0)
    {
        // Connection request widget
        mChildLiveboard->load(*iLiveboard);

        // Finish up
        delete iLiveboard;
    }
}


void MainController::show_liveboardwidget()
{
    // Check if the widget is loaded already
    if (mChildLiveboard == 0)
    {
        load_stations(this, SLOT(load_liveboardwidget(QMap<QString, StationPointer>*)));
        return;
    }

    mChildLiveboard->clear();
    mChildLiveboard->show();
}

void MainController::process_liveboardwidget_station(QString iStationId)
{
    load_liveboard(iStationId, this, SLOT(load_liveboardwidget_liveboard(LiveboardPointer*)));
}

void MainController::process_liveboardwidget(Liveboard::Departure iDeparture)
{

}
