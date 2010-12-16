//
// Configuration
//

// Includes
#include "application.h"
#include "api/storage/memorystorage.h"
#include <QDesktopServices>
#include <QDir>
#include <QApplication>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

Application* Application::mInstance = NULL;

Application::Application(int & argc, char ** argv) : QApplication(argc, argv), mAPI("Maemo", "0.99", &mStorage)
{
    // Singleton assertion (well, some singleton-hybrid, to be fair)
    Q_ASSERT(mInstance == NULL);
    mInstance = this;

    // Configure the application
    setOrganizationName("iRail");
    setOrganizationDomain("irail.be");
    setApplicationName("BeTrains");
    setApplicationVersion("0.99");

    // Translate the user interface
    Q_INIT_RESOURCE(translations);
    QTranslator tTranslator;
    tTranslator.load(settings().value("application/language", QLocale::system().name()).toString(), ":/translations");
    installTranslator(&tTranslator);

    // Construct the controllers
    mMain = new MainController(&mAPI);
    mLiveboard = new LiveboardController(&mAPI);
    mRequest = new RequestController(&mAPI);
    mConnection = new ConnectionController(&mAPI);
    mVehicle = new VehicleController(&mAPI);

    // Connect them
    connect(mMain, SIGNAL(launchLiveboard()), this, SLOT(_launchLiveboard()));
    connect(mMain, SIGNAL(launchLiveboard(LiveboardRequestPointer)), this, SLOT(_launchLiveboard(LiveboardRequestPointer)));
    connect(mLiveboard, SIGNAL(addHistory(QVariant)), mMain, SLOT(_addHistory(QVariant)));
    connect(mLiveboard, SIGNAL(launchVehicle(QString,Liveboard::Departure)), this, SLOT(_launchVehicleFromLiveboard(QString,Liveboard::Departure)));
    connect(mMain, SIGNAL(launchRequest()), this, SLOT(_launchRequest()));
    connect(mMain, SIGNAL(launchRequest(ConnectionRequestPointer)), this, SLOT(_launchRequest(ConnectionRequestPointer)));
    connect(mRequest, SIGNAL(addHistory(QVariant)), this, SLOT(_addHistory(QVariant)));
    connect(mRequest, SIGNAL(launchConnection(ConnectionRequestPointer)), this, SLOT(_launchConnection(ConnectionRequestPointer)));
    connect(mConnection, SIGNAL(launchVehicle(Connection::Line)), this, SLOT(_launchVehicleFromConnection(Connection::Line)));
    QTimer::singleShot(0, this, SLOT(run()));
    QObject::connect(this, SIGNAL(lastWindowClosed()), this, SLOT(close()));

    // Create directory structure
    mDataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if (mDataLocation.isEmpty())
            mDataLocation = QDir::homePath() + "/." + QApplication::applicationName();
    if (!QFile::exists(mDataLocation))
    {
        QDir tDirectory;
        tDirectory.mkpath(mDataLocation);
    }

    // Load eventual data
    QString tLocation = mDataLocation % "/storage.dat";
    QFile tFile(tLocation);
    bool tFileOpen = tFile.open(QIODevice::ReadOnly);
    if (tFileOpen)
    {
        QDataStream tStreamRead(&tFile);
        if (! mStorage.deserialize(tStreamRead, applicationVersion()))
            qWarning() << "! " << "Storage data corrupted, contents discarded";
        tFile.close();
    }
    else
        qWarning() << "! " << "Could not read storage data";
}

Application::~Application()
{
    delete mMain;
    mInstance = NULL;
}


//
// Singleton objects
//

Application *Application::instance()
{
    return mInstance;
}

QSettings& Application::settings()
{
    return instance()->mSettings;
}

Storage *Application::storage()
{
    return &(instance()->mStorage);
}


//
// UI events
//

void Application::run()
{
    mMain->showView();
}

void Application::close()
{
    // Save the storage
    QString tLocation = mDataLocation % "/storage.dat";
    QFile tFile(tLocation);
    bool tFileOpen = tFile.open(QIODevice::WriteOnly);
    if (tFileOpen)
    {
        QDataStream tStreamWrite(&tFile);
        mStorage.serialize(tStreamWrite, applicationVersion());
        tFile.close();
    }
    else
        qWarning() << "! " << "Could not write storage data";

    // Synchronize the settings
    settings().sync();
}


//
// Widget transitions
//

void Application::_launchLiveboard()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mLiveboard->showView(mMain);
}

void Application::_launchLiveboard(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mLiveboard->showView(mMain, iLiveboardRequest);
}


void Application::_launchRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mRequest->showView(mMain);
}

void Application::_launchRequest(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mRequest->showView(mMain, iConnectionRequest);
}

void Application::_launchConnection(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    // TODO emit addHistory(QVariant::fromValue(iConnectionRequest));

    mConnection->showView(mRequest, iConnectionRequest);
}

void Application::_launchVehicleFromConnection(Connection::Line iConnectionLine)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mVehicle->showView(mConnection, iConnectionLine);
}

void Application::_launchVehicleFromLiveboard(QString iStationId, Liveboard::Departure iLiveboardDeparture)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    Connection::Line tLine;
    tLine.departure.station = iStationId;
    tLine.departure.platform = iLiveboardDeparture.platform;
    tLine.departure.delay = iLiveboardDeparture.delay;
    tLine.departure.datetime = iLiveboardDeparture.datetime;
    tLine.arrival.station = iLiveboardDeparture.station;
    tLine.arrival.platform = 0;
    tLine.arrival.delay = 0;
    tLine.vehicle = iLiveboardDeparture.vehicle;

    mVehicle->showView(mLiveboard, tLine);
}
