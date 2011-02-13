//
// Configuration
//

// Includes
#include "maemoapplication.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MaemoApplication::MaemoApplication(int & argc, char ** argv) : Application(argc, argv, "Maemo", "1.0-beta-1")
{    
    // Translate the user interface
    Q_INIT_RESOURCE(i18n_maemo);
    QTranslator tTranslator;
    tTranslator.load(settings().value("application/language", QLocale::system().name()).toString(), ":/i18n_maemo");
    installTranslator(&tTranslator);
}
