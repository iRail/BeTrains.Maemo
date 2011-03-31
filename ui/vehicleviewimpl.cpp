//
// Configuration
//

// Includes
#include "vehicleviewimpl.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>
#include <QLabel>
#include "ui/auxiliary/delegates/vehiclestopdelegate.h"
#include "ui/global.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

VehicleViewImpl::VehicleViewImpl() : VehicleView()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    proxy = new GenericViewImpl(this);

    // Initialisation
    hide();
    init_ui();
}

VehicleViewImpl::~VehicleViewImpl()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}


//
// Controller actions
//

void VehicleViewImpl::reset()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // TODO: resuse the populateModel(empty, 0) code
    mViewDummy->setVisible(true);
    mView->setVisible(false);
}


void VehicleViewImpl::load(Connection::Line iLine)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    startLoader();
    tLine = iLine;
    emit downloadVehicle(iLine.vehicle);
}

void VehicleViewImpl::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = *iStations;
    delete iStations;

    populateModel(tLine, tVehicle);
    tVehicle.clear();
}

void VehicleViewImpl::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tVehicle = VehiclePointer(*iVehicle);
    delete iVehicle;    // TODO: clear this mess, don't emit QSharedPointer*
    emit downloadStations();
}


//
// Initialisation
//

void VehicleViewImpl::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(QString(tr("Connection detail")));

    // Main layout
    QVBoxLayout *tUILayout = new QVBoxLayout();
    tUILayout->setAlignment(Qt::AlignTop);
    centralWidget()->setLayout(tUILayout);


    // VIEW //

    // Create the stops model
    mModel = new QStandardItemModel(0, 1);

    // Create the stops list view
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setItemDelegate(new VehicleStopDelegate(mStations));
    mView->setSelectionMode(QAbstractItemView::NoSelection);
    tUILayout->addWidget(mView);

    // Create the departure list view dummy
    mViewDummy = new QLabel(tr("No stops to be shown."));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    tUILayout->addWidget(mViewDummy);
}

void VehicleViewImpl::populateModel(Connection::Line iLine, VehiclePointer iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // TODO: display some header based on iVehicle

    // Add the stops
    mModel->clear();
    bool tWithinEndpoints = false;
    int tCount = 0;
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
            tCount++;
        }
    }
    if (tCount)
    {
        mViewDummy->setVisible(false);
        mView->setVisible(true);
    }
    else
    {
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }

    // Fix the scroll location
    mView->scrollToTop();
}
