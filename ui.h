//
// Configuration
//

// Include guard
#ifndef UI_H
#define UI_H

// Includes
#include <QObject>
#include "cachedapi.h"
#include "api/storage/serializedstorage.h"
#include "ui/screens/maincontroller.h"

namespace iRail
{
    class UI : public QObject
    {
    Q_OBJECT
    public:
        // Construction and destruction
        UI();
        ~UI();
    public slots:
        void run();
        void close();
    private:
        // Member objects
        SerializedStorage mStorage;
        CachedAPI mAPI;
        MainController *mController;

        // Configuration data (TODO: move into some sort of configuration container)
        QString mDataLocation;
    };
}

#endif // UI_H
