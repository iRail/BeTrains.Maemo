//
// Configuration
//

// Include guard
#ifndef UI_H
#define UI_H

// Includes
#include <QObject>
#include "asyncapi.h"
#include "api/storage/memorystorage.h"
#include "ui/mainwidget.h"

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
        void warning(const QString& iMessage);
        void error(const QString& iMessage);
        void fatal(const QString& iMessage);
    private:
        MemoryStorage mStorage;
        AsyncAPI mAPI;
        MainWidget *mMain;
    };
}

#endif // UI_H
