//
// Configuration
//

// Includes
#include "connectiondetailwidget.h"
#include <QLabel>
#include <QStringBuilder>
#include <QStandardItemModel>
#include <QListView>
#include <QScrollArea>

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
    // Detail layout (and parent dummy widget)
    QWidget *tWidget = new QWidget();
    mUIDetailLayout = new QVBoxLayout(tWidget);

    // Scroll area
    QScrollArea* tScrollArea = new QScrollArea(this);
    tScrollArea->setWidgetResizable(true);
    tScrollArea->setWidget(tWidget);

    // Main layout
    mUILayout = new QVBoxLayout(this);
    mUILayout->addWidget(tScrollArea);
}

void ConnectionDetailWidget::update_ui(ConnectionPointer iConnection)
{
    // Window settings
    this->setWindowTitle(QString(tr("Detail - %1 to %2")
                                 .arg(iConnection->departure().station)
                                 .arg(iConnection->arrival().station))
                         );

    // Remove all items
    QLayoutItem* tItem;
    while (tItem = mUIDetailLayout->takeAt(0)) {
        if (tItem->widget())
            tItem->widget()->hide();
        delete tItem;
    }

    // Add new items
    foreach (Connection::Line tLine, iConnection->lines())
    {
        init_line(tLine);
    }

    // Add a spacer (setAlignment(Qt::AlignTop) doesn't seem to work)
    mUIDetailLayout->addStretch();
}

void ConnectionDetailWidget::init_line(const Connection::Line& iLine)
{
    // Title label
    QLabel* tPOILabel = new QLabel(iLine.departure.station % tr(" to ") % iLine.arrival.station);
    tPOILabel->setAlignment(Qt::AlignCenter);
    mUIDetailLayout->addWidget(tPOILabel);

    // Populate the stops list model
    QStandardItemModel* tModel = new QStandardItemModel(0, 1);

    // Create the stops listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(tModel);
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::NoSelection);
    tView->setFixedHeight(tView->sizeHint().height());  // TODO: disable drag completely
    tView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mUIDetailLayout->addWidget(tView);

    // Add some bogus data
    for (int i = 0; i < 3; i++)
    {
        QStandardItem *tItem = new QStandardItem("Stop " % QString::number(i));
        //tItem->setData(QVariant::fromValue(tConnection), ConnectionRole);
        tItem->setEditable(false);
        tModel->appendRow(tItem);
    }
}

void ConnectionDetailWidget::init_children()
{
}
