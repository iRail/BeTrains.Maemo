//
// Configuration
//

// Includes
#include "api/exception.h"
#include "maemoapplication.h"
#include "ui/mainviewimpl.h"
#include "ui/liveboardviewimpl.h"
#include "ui/requestviewimpl.h"
#include "ui/connectionviewimpl.h"
#include "ui/vehicleviewimpl.h"

// Namespaces
using namespace iRail;

int main(int argc, char *argv[])
{
    // Initialize the application
    MaemoApplication tApplication(argc, argv);

    // Configure the views
    tApplication.setMainView(new MainViewImpl());
    tApplication.setLiveboardView(new LiveboardViewImpl());
    tApplication.setRequestView(new RequestViewImpl());
    tApplication.setConnectionView(new ConnectionViewImpl());
    tApplication.setVehicleView(new VehicleViewImpl());
    tApplication.start();

    // Run the application
    try
    {
        return tApplication.exec();
    }
    catch (const Exception& iException)
    {
         QTextStream qerr(stderr);

         qerr << "---------------------------------------\n";
         qerr << "          UNTRAPPED EXCEPTION          \n";
         qerr << "---------------------------------------\n";
         qerr << "\n";
         qerr << iException.toString();

         return 0;
    }
}
