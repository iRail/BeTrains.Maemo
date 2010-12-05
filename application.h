//
// Configuration
//

// Include guard
#ifndef APPLICATION_H
#define APPLICATION_H

// Includes
#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QTranslator>
#include <QTimer>
#include "cachedapi.h"
#include "api/storage/serializedstorage.h"
#include "ui/screens/maincontroller.h"

namespace iRail
{
    class Application : public QApplication
    {
    Q_OBJECT
    public:
        // Construction and destruction
        Application(int & argc, char ** argv);
        ~Application();

        // Singleton objects
        static Application *instance();
        static QSettings &settings();
        static Storage* storage();

        // UI events
    public slots:
        void run();
        void close();

    private:        
        // Singleton object
        static Application *mInstance;

        // Member objects
        CachedAPI mAPI;
        MainController *mController;
        SerializedStorage mStorage;
        QSettings mSettings;

        // Configuration data (TODO: move into some sort of configuration container)
        QString mDataLocation;
    };
}


#endif // APPLICATION_H
