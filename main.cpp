//
// Configuration
//

// Includes
#include <QApplication>
#include <QTimer>
#include <QTranslator>
#include "ui.h"
#include "api/exception.h"
#include <QTextStream>

// Namespaces
using namespace iRail;

int main(int argc, char *argv[])
{
    QApplication tApplication(argc, argv);

    // Translate the user interface
    QTranslator tTranslator;
    tTranslator.load(QLocale::system().name(), ":/translations");
    tApplication.installTranslator(&tTranslator);

    // Set-up the user interface
    UI* tUI = new UI();
    QTimer::singleShot(0, tUI, SLOT(run()));

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
