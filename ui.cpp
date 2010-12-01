//
// Configuration
//

// Includes
#include "ui.h"
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

UI::UI() : mAPI("Maemo", "0.1", &mStorage)
{
    // Setup the main widget
    mController = new MainController(&mAPI);

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

UI::~UI()
{
    delete mController;
}

void UI::run()
{
    mController->showView();
}

void UI::close()
{
    // Save data
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
}
