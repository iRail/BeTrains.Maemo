//
// Configuration
//

// Include guard
#ifndef CONNECTIONRESULTWIDGET_H
#define CONNECTIONRESULTWIDGET_H

// Includes
#include <QWidget>
#include "api/connectionrequest.h"
#include "asyncapi.h"

namespace iRail
{
    class ConnectionResultWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionResultWidget(AsyncAPI* iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent);

    private:
        // Member data
        AsyncAPI* mAPI;
        ConnectionRequestPointer mConnectionRequest;

        // Initialization
        void init_ui();
        void init_children();
    };
}

#endif // CONNECTIONRESULTWIDGET_H
