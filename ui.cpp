//
// Configuration
//

// Includes
#include "ui.h"
#include "api/storage/memorystorage.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

UI::UI() : mAPI("Maemo", "0.1")
{
    // Set a memory cache
    mAPI.setStorage(&mStorage);

    // Connect message signals
    connect(&mAPI, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));
    connect(&mAPI, SIGNAL(warning(const QString&)), this, SLOT(warning(const QString&)));
    connect(&mAPI, SIGNAL(fatal(const QString&)), this, SLOT(fatal(const QString&)));

    // Setup the main widget
    mMain = new MainWidget(&mAPI);
}

UI::~UI()
{
    delete mMain;
}

void UI::run() {
    mMain->show();
}

void UI::warning(const QString& iMessage)
{
    QTextStream qout(stderr);

    qout << "WARNING: " << iMessage << "\n";
}

void UI::error(const QString& iMessage)
{
    QTextStream qout(stderr);

    qout << "ERROR: " << iMessage << "\n";
}

void UI::fatal(const QString& iMessage)
{
    QTextStream qout(stderr);

    qout << "FATAL: " << iMessage << "\n";
}
