//
// Configuration
//

// Includes
#include "connectiondetailwidget.h"
#include <QLabel>
#include <QStringBuilder>
#include <QStandardItemModel>
#include <QListView>
#include "auxiliary/delegates/connectionpoidelegate.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionDetailWidget::ConnectionDetailWidget(CachedAPI *iAPI, QWidget *iParent) : QScrollArea(iParent), mAPI(iAPI)
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
    // Parent widget
    QWidget *tWidget = new QWidget();
    setWidget(tWidget);
    setWidgetResizable(true);

    // Main layout
    mUILayout = new QVBoxLayout(this);
    tWidget->setLayout(mUILayout);
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
    while (tItem = mUILayout->takeAt(0))
    {
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
    mUILayout->addStretch();
}

void ConnectionDetailWidget::init_line(const Connection::Line& iLine)
{
    // Title label
    QLabel* tPOILabel = new QLabel(iLine.departure.station % tr(" to ") % iLine.arrival.station);
    tPOILabel->setAlignment(Qt::AlignCenter);
    mUILayout->addWidget(tPOILabel);

    // Populate the stops list model
    QStandardItemModel* tModel = new QStandardItemModel(0, 1);

    // Create the stops listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(tModel);
    tView->setItemDelegate(new ConnectionPOIDelegate());
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::NoSelection);
    tView->setFixedHeight(tView->sizeHint().height());  // TODO: disable drag completely
    tView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mUILayout->addWidget(tView);

    // Add the endpoints (TODO: the stops between also, needs line lookup)
    QStandardItem *tItem = new QStandardItem();
    tItem->setEditable(false);
    tItem->setData(QVariant::fromValue(iLine.departure), ConnectionPOIRole);
    tModel->appendRow(tItem);
    tItem = new QStandardItem();
    tItem->setEditable(false);
    tItem->setData(QVariant::fromValue(iLine.arrival), ConnectionPOIRole);
    tModel->appendRow(tItem);
}

void ConnectionDetailWidget::init_children()
{
}
