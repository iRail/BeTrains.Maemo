//
// Configuration
//

// Include guard
#ifndef UI_H
#define UI_H

// Includes
#include <QObject>
#include <QSettings>
#include "cachedapi.h"
#include "api/storage/serializedstorage.h"
#include "ui/screens/maincontroller.h"

namespace iRail
{
// TODO: make this the application (extend QApplication)
    class UI : public QObject
    {
    Q_OBJECT
    public:
        // Construction and destruction
        UI();
        ~UI();

        // Singleton objects
        static QSettings &settings();

    public slots:
        void run();
        void close();
    private:
        // Member objects
        SerializedStorage mStorage;
        CachedAPI mAPI;
        MainController *mController;
        static QSettings &mSettings;

        // Configuration data (TODO: move into some sort of configuration container)
        QString mDataLocation;
    };
}


#endif // UI_H
