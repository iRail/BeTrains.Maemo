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

void VehicleView::load(Connection::Line iLine)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tLine = iLine;
    emit downloadVehicle(iLine.vehicle);
}

void VehicleView::load(const QMap<QString, StationPointer>& iStations, Connection::Line iLine, VehiclePointer iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    stopLoader();
    mStations = iStations;

    // Show the results
    populateModel(iLine, iVehicle);
}


//
// Controller actions
//

void VehicleView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iStations, tLine, tVehicle);
    delete iStations;
    tVehicle.clear();
}

void VehicleView::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tVehicle = VehiclePointer(*iVehicle);
    delete iVehicle;    // TODO: clear this mess, don't emit QSharedPointer*
    emit downloadStations();
}


//
// Initialisation
//

void VehicleView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    this->setWindowTitle(QString(tr("Connection detail")));

    // Scroll area
    QVBoxLayout* mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);
    mUIScrollArea = new QScrollArea(this);
    mUILayout->addWidget(mUIScrollArea);

    // Parent widget
    QWidget *tWidget = new QWidget();
    mUIScrollArea->setWidget(tWidget);
    mUIScrollArea->setWidgetResizable(true);

    // Main layout
    mUIScrollLayout = new QVBoxLayout(mUIScrollArea);
    tWidget->setLayout(mUIScrollLayout);

    // Create the stops model
    mModel = new QStandardItemModel(0, 1);

    // Create the stops listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setItemDelegate(new VehicleStopDelegate(mStations));
    mView->setSelectionMode(QAbstractItemView::NoSelection);
    mView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mUIScrollLayout->addWidget(mView);

    // HACK (without fixedheight we could use sizepolicy)
    mUIScrollLayout->addStretch();
}

void VehicleView::populateModel(Connection::Line iLine, VehiclePointer iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Title label
    /*
    QFont tFont;
    tFont.setPointSize(18);
    tFont.setBold(true);
    QLabel* tPOILabel = new QLabel(mStations[iLine.departure.station]->name() % tr(" to ") % mStations[iLine.arrival.station]->name());
    tPOILabel->setFont(tFont);
    tPOILabel->setAlignment(Qt::AlignCenter);
    mUIScrollLayout->addWidget(tPOILabel);
    */

    // Add the stops
    mModel->clear();
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
            mModel->appendRow(tItem);
        }
    }

    // TODO: configure the QListView to be expanding within the QScrollArea
    // SizePolicy doesn't work
    // Setting the fixed height works for 2 items, but not more, as it _always_
    // seem to return 192, and doesn't update after adding items/updating the model
    mView->setFixedHeight(70*mModel->rowCount());   // HACK

    // Fix the scroll location
    mUIScrollArea->ensureVisible(0, 0, 0, 0);
}
