//
// Configuration
//

// Includes
#include "connectionview.h"
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

ConnectionView::ConnectionView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);

    mChildConnectionResult = 0;

    this->hide();
}

ConnectionView::~ConnectionView()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void ConnectionView::showUI(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionResult(iConnectionRequest);
    this->show();
}


//
// UI events
//

void ConnectionView::_showConnectionResult(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    emit downloadConnections(iConnectionRequest);
}

void ConnectionView::_showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionResult == 0)
    {
        // Connection request widget
        mChildConnectionResult = new ConnectionResultWidget(iStations, this);
        mUILayout->addWidget(mChildConnectionResult);
        connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SIGNAL(launchVehicle(ConnectionPointer)));
    }

    // Show the results
    mChildConnectionResult->show();
    mChildConnectionResult->load(iConnections);
}


//
// Controller actions
//

void ConnectionView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionResult(*iStations, *tConnections);
    delete iStations;
    delete tConnections;
}

void ConnectionView::setConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    tConnections = iConnections;
    emit downloadStations();
}

void ConnectionView::showError(const QString &iError)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}
