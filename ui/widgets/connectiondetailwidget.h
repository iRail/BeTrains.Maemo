//
// Configuration
//

// Include guard
#ifndef CONNECTIONDETAILWIDGET_H
#define CONNECTIONDETAILWIDGET_H

// Includes
#include "cachedapi.h"
#include "global.h"
#include <QVBoxLayout>
#include <QScrollArea>

namespace iRail
{
    class ConnectionDetailWidget : public QScrollArea
    {
    Q_OBJECT
    public:
        explicit ConnectionDetailWidget(CachedAPI* iAPI, QWidget *iParent);
        ~ConnectionDetailWidget();

        // Public slots
    public slots:
        void setConnection(ConnectionPointer iConnection);

    private:
        // Member data
        CachedAPI* mAPI;

        // Initialization
    private:
        void init_ui();
        void update_ui(ConnectionPointer iConnection);
        void init_children();
        void init_line(const Connection::Line& iLine);

        // UI members
    private:
        QVBoxLayout* mUILayout;
    };
}

#endif // CONNECTIONDETAILWIDGET_H
