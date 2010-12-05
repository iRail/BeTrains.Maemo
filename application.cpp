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

Application::Application(int & argc, char ** argv) : QApplication(argc, argv), mAPI("Maemo", "0.1", &mStorage)
{
    // Singleton assertion (well, some singleton-hybrid, to be fair)
    Q_ASSERT(mInstance == NULL);
    mInstance = this;

    // Configure the application
    setOrganizationName("iRail");
    setOrganizationDomain("irail.be");
    setApplicationName("BeTrains");

    // Translate the user interface
    Q_INIT_RESOURCE(translations);
    QTranslator tTranslator;
    tTranslator.load(settings().value("application/language", QLocale::system().name()).toString(), ":/translations");
    installTranslator(&tTranslator);

    // Setup the main widget
    mController = new MainController(&mAPI);
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
        mStorage.deserialize(tStreamRead);
        tFile.close();
    }
    else
        qWarning() << "! " << "Could not read storage data";
}

Application::~Application()
{
    delete mController;
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
    mController->showView();
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
        mStorage.serialize(tStreamWrite);
        tFile.close();
    }
    else
        qWarning() << "! " << "Could not write storage data";

    // Synchronize the settings
    settings().sync();
}
