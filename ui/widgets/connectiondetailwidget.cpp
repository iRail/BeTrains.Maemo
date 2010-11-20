//
// Configuration
//

// Includes
#include <QDebug>
#include "connectiondetailwidget.h"
#include <QLabel>
#include <QStringBuilder>
#include <QStandardItemModel>
#include <QListView>
#include "ui/auxiliary/delegates/vehiclestopdelegate.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionDetailWidget::ConnectionDetailWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent) : QScrollArea(iParent), mStations(iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Initialisation
    init_ui();
    init_children();
}

ConnectionDetailWidget::~ConnectionDetailWidget()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void ConnectionDetailWidget::load(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Alter the UI
    update_ui(iConnection, iVehicles);
}


//
// Initialisation
//

void ConnectionDetailWidget::init_ui()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Parent widget
    QWidget *tWidget = new QWidget();
    setWidget(tWidget);
    setWidgetResizable(true);

    // Main layout
    mUILayout = new QVBoxLayout(this);
    tWidget->setLayout(mUILayout);
}

void ConnectionDetailWidget::update_ui(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Window settings
    this->setWindowTitle(QString(tr("Connection detail")));

    // Remove all items
    QLayoutItem* tItem;
    while (mUILayout->count())
    {
        tItem = mUILayout->takeAt(0);
        if (tItem->widget())
            tItem->widget()->hide();
        delete tItem;
    }

    // Add new items
    foreach (Connection::Line tLine, iConnection->lines())
    {
        init_line(tLine, iVehicles[tLine.vehicle]);
    }

    // Add a spacer (setAlignment(Qt::AlignTop) doesn't seem to work)
    mUILayout->addStretch();
}

void ConnectionDetailWidget::init_line(const Connection::Line& iLine, const VehiclePointer& iVehicle)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Title label
    QFont tFont;
    tFont.setPointSize(18);
    tFont.setBold(true);
    QLabel* tPOILabel = new QLabel(mStations[iLine.departure.station]->name() % tr(" to ") % mStations[iLine.arrival.station]->name());
    tPOILabel->setFont(tFont);
    tPOILabel->setAlignment(Qt::AlignCenter);
    mUILayout->addWidget(tPOILabel);

    // Populate the stops list model
    QStandardItemModel* tModel = new QStandardItemModel(0, 1);

    // Create the stops listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(tModel);
    tView->setItemDelegate(new VehicleStopDelegate(mStations));
    tView->setSelectionMode(QAbstractItemView::NoSelection);
    tView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mUILayout->addWidget(tView);

    // Add the stops
    bool tWithinEndpoints = false;
    foreach (Vehicle::Stop tStop, iVehicle->stops())
    {
        if (!tWithinEndpoints && tStop.station == iLine.departure.station)
        {
            tStop.platform = iLine.departure.platform;
            tWithinEndpoints = true;
        }
        bool tDisplay = tWithinEndpoints;
        if (tWithinEndpoints && tStop.station == iLine.arrival.station)
        {
            tStop.platform = iLine.arrival.platform;
            tWithinEndpoints = false;
        }

        if (tDisplay)
        {
            QStandardItem *tItem = new QStandardItem();
            tItem->setData(QVariant::fromValue(tStop), VehicleStopRole);
            tModel->appendRow(tItem);
        }
    }

    // TODO: configure the QListView to be expanding within the QScrollArea
    // SizePolicy doesn't work
    // Setting the fixed height works for 2 items, but not more, as it _always_
    // seem to return 192, and doesn't update after adding items/updating the model
    tView->setFixedHeight(70*tModel->rowCount());   // HACK

    // Add some space
    mUILayout->addSpacing(42);
}

void ConnectionDetailWidget::init_children()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

}
