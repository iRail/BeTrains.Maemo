//
// Configuration
//

// Includes
#include "api/exception.h"
#include "application.h"

// Namespaces
using namespace iRail;

int main(int argc, char *argv[])
{
    // Initialize the application
    Application tApplication(argc, argv);

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
