//
// Configuration
//

// Includes
#include "vehicleview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>
#include <QLabel>
#include <QStandardItemModel>
#include <QListView>
#include "ui/auxiliary/delegates/vehiclestopdelegate.h"
#include "ui/global.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

VehicleView::VehicleView(QWidget* iParent) : GenericView(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Initialisation
    this->hide();
    init_ui();
}

VehicleView::~VehicleView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}

void VehicleView::load(ConnectionPointer iConnection)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    startLoader();

    tConnection = iConnection;
    tVehicles = new QMap<QString, VehiclePointer>();
    emit downloadVehicle(iConnection->lines().at(0).vehicle);
}

void VehicleView::load(const QMap<QString, StationPointer>& iStations, ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    stopLoader();
    mStations = iStations;

    // Show the results
    update_ui(iConnection, iVehicles);
}


//
// Controller actions
//

void VehicleView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iStations, tConnection, *tVehicles);
    delete iStations;
    delete tVehicles;
    tConnection.clear();
}

void VehicleView::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tVehicles->insert((*iVehicle)->id(), *iVehicle);
    if (tConnection->lines().count() > tVehicles->count())
        emit downloadVehicle(tConnection->lines().at(tVehicles->size()).vehicle);
    else
        emit downloadStations();
}


//
// Initialisation
//

void VehicleView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Scroll area
    QVBoxLayout* mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);
    QScrollArea* mUIScrollArea = new QScrollArea(this);
    mUILayout->addWidget(mUIScrollArea);

    // Parent widget
    QWidget *tWidget = new QWidget();
    mUIScrollArea->setWidget(tWidget);
    mUIScrollArea->setWidgetResizable(true);

    // Main layout
    mUIScrollLayout = new QVBoxLayout(mUIScrollArea);
    tWidget->setLayout(mUIScrollLayout);
}

void VehicleView::update_ui(ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    this->setWindowTitle(QString(tr("Connection detail")));

    // Remove all items
    QLayoutItem* tItem;
    while (mUIScrollLayout->count())
    {
        tItem = mUIScrollLayout->takeAt(0);
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
    mUIScrollLayout->addStretch();
}

void VehicleView::init_line(const Connection::Line& iLine, const VehiclePointer& iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Title label
    QFont tFont;
    tFont.setPointSize(18);
    tFont.setBold(true);
    QLabel* tPOILabel = new QLabel(mStations[iLine.departure.station]->name() % tr(" to ") % mStations[iLine.arrival.station]->name());
    tPOILabel->setFont(tFont);
    tPOILabel->setAlignment(Qt::AlignCenter);
    mUIScrollLayout->addWidget(tPOILabel);

    // Populate the stops list model
    QStandardItemModel* tModel = new QStandardItemModel(0, 1);

    // Create the stops listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(tModel);
    tView->setItemDelegate(new VehicleStopDelegate(mStations));
    tView->setSelectionMode(QAbstractItemView::NoSelection);
    tView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mUIScrollLayout->addWidget(tView);

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
    mUIScrollLayout->addSpacing(42);
}
