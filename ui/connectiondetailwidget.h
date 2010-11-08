//
// Configuration
//

// Include guard
#ifndef CONNECTIONDETAILWIDGET_H
#define CONNECTIONDETAILWIDGET_H

// Includes
#include <QWidget>
#include "cachedapi.h"
#include <QVBoxLayout>
#include "global.h"

namespace iRail
{
    class ConnectionDetailWidget : public QWidget
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

        // UI members
    private:
        QVBoxLayout* mUILayout;
    };
}

#endif // CONNECTIONDETAILWIDGET_H