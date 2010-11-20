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

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

VehicleView::VehicleView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);

    mChildConnectionDetail = 0;

    this->hide();
}

VehicleView::~VehicleView()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void VehicleView::showUI(ConnectionPointer iConnection)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionDetail(iConnection);
    this->show();
}


//
// UI events
//

void VehicleView::_showConnectionDetail(ConnectionPointer iConnection)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    tConnection = iConnection;
    tVehicles = new QMap<QString, VehiclePointer>();
    emit downloadVehicle(iConnection->lines().at(0).vehicle);
}

void VehicleView::_showConnectionDetail(const QMap<QString, StationPointer>& iStations, ConnectionPointer iConnection, const QMap<QString, VehiclePointer>& iVehicles)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionDetail == 0)
    {
        // Connection request widget
        mChildConnectionDetail = new ConnectionDetailWidget(iStations, this);
        mUILayout->addWidget(mChildConnectionDetail);
    }

    // Show the results
    mChildConnectionDetail->show();
    mChildConnectionDetail->load(iConnection, iVehicles);
}


//
// Controller actions
//

void VehicleView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionDetail(*iStations, tConnection, *tVehicles);
    delete iStations;
    delete tVehicles;
    tConnection.clear();
}

void VehicleView::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    tVehicles->insert((*iVehicle)->id(), *iVehicle);
    if (tConnection->lines().count() > tVehicles->count())
        emit downloadVehicle(tConnection->lines().at(tVehicles->size()).vehicle);
    else
        emit downloadStations();
}

void VehicleView::showError(const QString &iError)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}
