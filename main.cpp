//
// Configuration
//

// Includes
#include <QObject>
#include <QApplication>
#include <QTimer>
#include <QTranslator>
#include "ui.h"
#include "api/exception.h"
#include <QTextStream>
#include <QDir>
#include <QSettings>

// Namespaces
using namespace iRail;

int main(int argc, char *argv[])
{
    // Initialize the application
    QApplication tApplication(argc, argv);
    tApplication.setOrganizationName("iRail");
    tApplication.setOrganizationDomain("irail.be");
    tApplication.setApplicationName("BeTrains");

    // Translate the user interface
    Q_INIT_RESOURCE(translations);
    QTranslator tTranslator;
    tTranslator.load(UI::settings().value("application/language", QLocale::system().name()).toString(), ":/translations");
    tApplication.installTranslator(&tTranslator);

    // Set-up the user interface
    UI* tUI = new UI();
    QTimer::singleShot(0, tUI, SLOT(run()));
    QObject::connect(&tApplication, SIGNAL(lastWindowClosed()), tUI, SLOT(close()));

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
