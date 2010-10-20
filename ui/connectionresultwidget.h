//
// Configuration
//

// Include guard
#ifndef CONNECTIONRESULTWIDGET_H
#define CONNECTIONRESULTWIDGET_H

// Includes
#include <QWidget>
#include "api/connectionrequest.h"
#include "cachedapi.h"

namespace iRail
{
    class ConnectionResultWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionResultWidget(CachedAPI* iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent);

    private:
        // Member data
        CachedAPI* mAPI;
        ConnectionRequestPointer mConnectionRequest;

        // Initialization
        void init_ui();
        void init_children();
    };
}

#endif // CONNECTIONRESULTWIDGET_H
