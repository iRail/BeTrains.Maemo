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

MaemoApplication::MaemoApplication(int & argc, char ** argv) : Application(argc, argv, "Maemo", "1.0.2")
{
    // Translate the user interface
    QTranslator *tInterfaceTranslator = new QTranslator();
    if (! tInterfaceTranslator->load(settings().value("application/language", QLocale::system().name()).toString(), ":/i18n"))
        qWarning() << "! " << "Interface translation failed";
    else
        qDebug() << "- " << "Interface translation succeeded";
    installTranslator(tInterfaceTranslator);
}
