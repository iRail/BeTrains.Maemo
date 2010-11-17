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

UI::UI() : mAPI("Maemo", "0.1", &mStorage)
{
    // Setup the main widget
    mMain = new MainWindow(&mAPI);
}

UI::~UI()
{
    delete mMain;
}

void UI::run() {
    mMain->show();
}
