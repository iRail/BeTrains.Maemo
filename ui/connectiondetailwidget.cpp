//
// Configuration
//

// Includes
#include "connectiondetailwidget.h"
#include <QLabel>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionDetailWidget::ConnectionDetailWidget(CachedAPI *iAPI, QWidget *iParent) : QWidget(iParent), mAPI(iAPI)
{
    // Initialisation
    init_ui();
    init_children();
}

ConnectionDetailWidget::~ConnectionDetailWidget()
{
}


//
// Public slots
//

void ConnectionDetailWidget::setConnection(ConnectionPointer iConnection)
{
    // Alter the UI
    update_ui(iConnection);
}


//
// Initialisation
//


void ConnectionDetailWidget::init_ui()
{
    // Main layout
    mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);
}

void ConnectionDetailWidget::update_ui(ConnectionPointer iConnection)
{
    // Window settings
    this->setWindowTitle(QString(tr("Detail - %1 to %2")
                                 .arg(iConnection->transfer().departure.station)
                                 .arg(iConnection->transfer().arrival.station))
                         );

    // Remove all items
    while (mUILayout->count())
    {
        QLayoutItem* tOldItem = mUILayout->itemAt(0);
        mUILayout->removeItem(tOldItem);
        delete tOldItem;
    }

    // Add new items
    if (iConnection->transfers().size() > 0)
    {
        Connection::Transfer tConnectionTransfer;
        foreach (tConnectionTransfer, iConnection->transfers())
        {
            QLabel* tPOILabel = new QLabel(tConnectionTransfer.departure.station % tr(" to ") % tConnectionTransfer.arrival.station);
            tPOILabel->setAlignment(Qt::AlignCenter);
            mUILayout->addWidget(tPOILabel);
        }
    }
    else
    {
        QLabel* tPOILabel = new QLabel(iConnection->transfer().departure.station % tr(" to ") % iConnection->transfer().arrival.station);
        tPOILabel->setAlignment(Qt::AlignCenter);
        mUILayout->addWidget(tPOILabel);
    }

}

void ConnectionDetailWidget::init_children()
{
}
