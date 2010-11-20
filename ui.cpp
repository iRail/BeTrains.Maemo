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
    mController = new MainController(&mAPI);
}

UI::~UI()
{
    delete mController;
}

void UI::run() {
    mController->show();
}
