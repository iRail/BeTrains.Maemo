//
// Configuration
//

// Include guard
#ifndef UI_H
#define UI_H

// Includes
#include <QObject>
#include "cachedapi.h"
#include "api/storage/memorystorage.h"
#include "ui/mainwindow.h"

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
    private:
        MemoryStorage mStorage;
        CachedAPI mAPI;
        MainWidget *mMain;
    };
}

#endif // UI_H
