//
// Configuration
//

// Includes
#include "connectionresultwidget.h"
#include <QVBoxLayout>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionResultWidget::ConnectionResultWidget(CachedAPI *iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent) : QWidget(iParent), mAPI(iAPI), mConnectionRequest(iConnectionRequest)
{
    // Initialisation
    init_ui();
    init_children();
}



//
// Initialisation
//


void ConnectionResultWidget::init_ui()
{
    // Window settings
    this->setWindowTitle(QString(tr("Connections - %1 to %2")
                                 .arg(mConnectionRequest->getOrigin())
                                 .arg(mConnectionRequest->getDestination()))
                         );

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);
}

void ConnectionResultWidget::init_children()
{

}
